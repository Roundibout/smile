/*
File:
    renderer_gl.cpp
Authors:
    Lucas
Purpose:
    Implementation of the Renderer implementation class for OpenGL
License:
    MIT (see LICENSE file)
*/

#include "renderer_gl.hpp"

// Simple orthographic matrix builder
static std::array<float, 16> makeOrtho(float left, float right, float bottom, float top) {
    std::array<float, 16> mat = {0};

    mat[0]  = 2.0f / (right - left);
    mat[5]  = 2.0f / (top - bottom);
    mat[10] = -1.0f;
    mat[12] = -(right + left) / (right - left);
    mat[13] = -(top + bottom) / (top - bottom);
    mat[15] = 1.0f;

    return mat;
}

RendererGL::RendererGL(WindowImpl* w) : RendererImpl(w) {
    w->bindGLContext();
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    shaders = ShaderManagerGL();
    shaders.loadShader("quad", "assets/shaders/quad.vert", "assets/shaders/quad.frag");
    shaders.loadShader("rounded", "assets/shaders/rounded.vert", "assets/shaders/rounded.frag");
    shaders.loadShader("text", "assets/shaders/text.vert", "assets/shaders/text.frag");
    
    // Create VAO/VBO/EBO for quad batching
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);

    glBindVertexArray(quadVAO);

    // Allocate buffer for vertices (dynamic draw for live updates which is important)
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, maxQuads * 4 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    // Precompute and upload indices for maxQuads
    batchIndices.resize(maxQuads * 6);
    for (size_t i = 0; i < maxQuads; ++i) {
        unsigned int offset = i * 4;
        batchIndices[i*6 + 0] = offset + 0;
        batchIndices[i*6 + 1] = offset + 1;
        batchIndices[i*6 + 2] = offset + 2;
        batchIndices[i*6 + 3] = offset + 0;
        batchIndices[i*6 + 4] = offset + 2;
        batchIndices[i*6 + 5] = offset + 3;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, batchIndices.size() * sizeof(unsigned int), batchIndices.data(), GL_STATIC_DRAW);

    // Set up vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Create VAO/VBO/EBO for rounded quad batching
    glGenVertexArrays(1, &roundedVAO);
    glGenBuffers(1, &roundedVBO);
    glGenBuffers(1, &roundedEBO);

    glBindVertexArray(roundedVAO);

    // Allocate buffer for vertices (dynamic draw for live updates which is important)
    glBindBuffer(GL_ARRAY_BUFFER, roundedVBO);
    glBufferData(GL_ARRAY_BUFFER, maxRoundeds * 4 * sizeof(RoundedVertex), nullptr, GL_DYNAMIC_DRAW);

    // Precompute and upload indices for maxRoundeds
    roundedBatchIndices.resize(maxRoundeds * 6);
    for (size_t i = 0; i < maxRoundeds; ++i) {
        unsigned int offset = i * 4;
        roundedBatchIndices[i*6 + 0] = offset + 0;
        roundedBatchIndices[i*6 + 1] = offset + 1;
        roundedBatchIndices[i*6 + 2] = offset + 2;
        roundedBatchIndices[i*6 + 3] = offset + 0;
        roundedBatchIndices[i*6 + 4] = offset + 2;
        roundedBatchIndices[i*6 + 5] = offset + 3;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roundedEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, roundedBatchIndices.size() * sizeof(unsigned int), roundedBatchIndices.data(), GL_STATIC_DRAW);

    // Set up vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(RoundedVertex), (void*)offsetof(RoundedVertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(RoundedVertex), (void*)offsetof(RoundedVertex, rectPosition));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(RoundedVertex), (void*)offsetof(RoundedVertex, rectSize));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(RoundedVertex), (void*)offsetof(RoundedVertex, color));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(RoundedVertex), (void*)offsetof(RoundedVertex, corners));
    glEnableVertexAttribArray(4);

    glBindVertexArray(0);

    // Create VAO/VBO/EBO for text batching
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glGenBuffers(1, &textEBO);

    glBindVertexArray(textVAO);

    // Allocate buffer for text vertices
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, maxTextQuads * 4 * sizeof(TextVertex), nullptr, GL_DYNAMIC_DRAW);

    // Precompute and upload indices for maxTextQuads
    textBatchIndices.resize(maxTextQuads * 6);
    for (size_t i = 0; i < maxTextQuads; ++i) {
        unsigned int offset = i * 4;
        textBatchIndices[i*6 + 0] = offset + 0;
        textBatchIndices[i*6 + 1] = offset + 1;
        textBatchIndices[i*6 + 2] = offset + 2;
        textBatchIndices[i*6 + 3] = offset + 0;
        textBatchIndices[i*6 + 4] = offset + 2;
        textBatchIndices[i*6 + 5] = offset + 3;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, textBatchIndices.size() * sizeof(unsigned int), textBatchIndices.data(), GL_STATIC_DRAW);

    // Set up vertex attributes for text
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)offsetof(TextVertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)offsetof(TextVertex, texCoord));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)offsetof(TextVertex, color));
    glEnableVertexAttribArray(2);

    // Crap to stop error
    shaders.useShader("text");

    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);

    GLuint dummyTex;
    glGenTextures(1, &dummyTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, dummyTex);

    TextVertex dummyVertex{};
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TextVertex), &dummyVertex);

    shaders.setUniformInt("text", "uTexture", 0);

    glBindVertexArray(0);
}

void RendererGL::flushQuadBatch() {
    if (quadCount == 0) return;

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, batchVertices.size() * sizeof(Vertex), batchVertices.data());

    shaders.useShader("quad");
    shaders.setUniformMat4("quad", "uProjection", currentProjection.data());
    glDrawElements(GL_TRIANGLES, quadCount * 6, GL_UNSIGNED_INT, 0);

    batchVertices.clear();
    quadCount = 0;

    glBindVertexArray(0);
}

void RendererGL::flushRoundedBatch() {
    if (roundedCount == 0) return;

    glBindVertexArray(roundedVAO);
    glBindBuffer(GL_ARRAY_BUFFER, roundedVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, roundedBatchVertices.size() * sizeof(RoundedVertex), roundedBatchVertices.data());

    shaders.useShader("rounded");
    shaders.setUniformMat4("rounded", "uProjection", currentProjection.data());
    glDrawElements(GL_TRIANGLES, roundedCount * 6, GL_UNSIGNED_INT, 0);

    roundedBatchVertices.clear();
    roundedCount = 0;

    glBindVertexArray(0);
}

void RendererGL::flushTextBatch() {
    if (textQuadCount == 0) return;

    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, textBatchVertices.size() * sizeof(TextVertex), textBatchVertices.data());

    shaders.useShader("text");
    shaders.setUniformMat4("text", "uProjection", currentProjection.data());
    
    // For simplicity, we'll render each glyph separately
    // A more advanced implementation would batch glyphs by texture
    for (size_t i = 0; i < textQuadCount; ++i) {
        // We need to bind the appropriate texture here
        // For now, this is a simplified version (we will sort by texture later)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(unsigned int)));
    }

    textBatchVertices.clear();
    textQuadCount = 0;

    glBindVertexArray(0);
}

void RendererGL::beginFrame(float scale) {
    window->makeGLCurrent();
    
    currentScale = scale;

    // Get and set viewport size
    Vector2 size = window->getSize();
    glViewport(0, 0, int(size.x), int(size.y));

    // Get orthographic projection
    currentProjection = makeOrtho(0.0f, size.x, 0.0f, size.y);

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Clear the screen
    Color4 color = window->getConfig().color;
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    batchVertices.clear();
    roundedBatchVertices.clear();
    textBatchVertices.clear();
    quadCount = 0;
    roundedCount = 0;
    textQuadCount = 0;

    // Set uniforms for shaders
    shaders.setUniformMat4("quad", "uProjection", currentProjection.data());
    shaders.setUniformMat4("rounded", "uProjection", currentProjection.data());
    shaders.setUniformMat4("text", "uProjection", currentProjection.data());
}

void RendererGL::drawRect(const UILayout& layout, const UIBounds& bounds, const Color4& color) {
    if (quadCount >= maxQuads) flushQuadBatch();
    if (roundedCount > 0) flushRoundedBatch();
    if (textQuadCount > 0) flushTextBatch();

    AbsoluteLayout resolved = resolveLayout(layout, bounds);
    Vector2 position = resolved.rect.position;
    Vector2 size = resolved.rect.size;

    Vertex v0{{position.x, position.y}, color};
    Vertex v1{{position.x + size.x, position.y}, color};
    Vertex v2{{position.x + size.x, position.y + size.y}, color};
    Vertex v3{{position.x, position.y + size.y}, color};

    batchVertices.push_back(v0);
    batchVertices.push_back(v1);
    batchVertices.push_back(v2);
    batchVertices.push_back(v3);

    ++quadCount;
}

void RendererGL::drawStrokeRect(const UILayout& layout, const UIBounds& bounds, const Color4& color, int stroke, const Color4& strokeColor, const UIStrokeAlignment& strokeAlignment) {
    stroke = static_cast<int>(std::round(stroke * currentScale));
}

void RendererGL::drawRoundedRect(const UILayout& layout, const UIBounds& bounds, const Color4& color) {
    if (roundedCount >= maxRoundeds) flushRoundedBatch();
    if (quadCount > 0) flushQuadBatch();
    if (textQuadCount > 0) flushTextBatch();

    AbsoluteLayout resolved = resolveLayout(layout, bounds);
    Vector2 position = resolved.rect.position;
    Vector2 size = resolved.rect.size;

    RoundedVertex v0{{0.0f, 0.0f}, position, size, color, {resolved.cornerLB, resolved.cornerRB, resolved.cornerRT, resolved.cornerLT}};
    RoundedVertex v1{{1.0f, 0.0f}, position, size, color, {resolved.cornerLB, resolved.cornerRB, resolved.cornerRT, resolved.cornerLT}};
    RoundedVertex v2{{1.0f, 1.0f}, position, size, color, {resolved.cornerLB, resolved.cornerRB, resolved.cornerRT, resolved.cornerLT}};
    RoundedVertex v3{{0.0f, 1.0f}, position, size, color, {resolved.cornerLB, resolved.cornerRB, resolved.cornerRT, resolved.cornerLT}};

    roundedBatchVertices.push_back(v0);
    roundedBatchVertices.push_back(v1);
    roundedBatchVertices.push_back(v2);
    roundedBatchVertices.push_back(v3);

    ++roundedCount;
}

void RendererGL::drawRoundedStrokeRect(const UILayout& layout, const UIBounds& bounds, const Color4& color, int stroke, const Color4& strokeColor, const UIStrokeAlignment& strokeAlignment) {
    if (roundedCount >= maxRoundeds) flushRoundedBatch();
    if (quadCount > 0) flushQuadBatch();
    if (textQuadCount > 0) flushTextBatch();

    stroke = static_cast<int>(std::round(stroke * currentScale));

    AbsoluteLayout resolved = resolveLayout(layout, bounds);
    Vector2 position = resolved.rect.position;
    Vector2 size = resolved.rect.size;

    { // Stroke
        RoundedVertex vertices[4];

        if (strokeAlignment == UIStrokeAlignment::Outside) {
            Vector2 offsetPosition = position - Vector2(stroke, stroke);
            Vector2 offsetSize = size + Vector2(stroke * 2, stroke * 2);
            float offsetCornerLB = resolved.cornerLB + stroke;
            float offsetCornerRB = resolved.cornerRB + stroke;
            float offsetCornerRT = resolved.cornerRT + stroke;
            float offsetCornerLT = resolved.cornerLT + stroke;
            vertices[0] = RoundedVertex{{0.0f, 0.0f}, offsetPosition, offsetSize, strokeColor, {offsetCornerLB, offsetCornerRB, offsetCornerRT, offsetCornerLT}};
            vertices[1] = RoundedVertex{{1.0f, 0.0f}, offsetPosition, offsetSize, strokeColor, {offsetCornerLB, offsetCornerRB, offsetCornerRT, offsetCornerLT}};
            vertices[2] = RoundedVertex{{1.0f, 1.0f}, offsetPosition, offsetSize, strokeColor, {offsetCornerLB, offsetCornerRB, offsetCornerRT, offsetCornerLT}};
            vertices[3] = RoundedVertex{{0.0f, 1.0f}, offsetPosition, offsetSize, strokeColor, {offsetCornerLB, offsetCornerRB, offsetCornerRT, offsetCornerLT}};
        } else if (strokeAlignment == UIStrokeAlignment::Middle) {
            Vector2 offsetPosition = position - Vector2(stroke / 2, stroke / 2);
            Vector2 offsetSize = size + Vector2(stroke, stroke);
            float offsetCornerLB = resolved.cornerLB + stroke / 2;
            float offsetCornerRB = resolved.cornerRB + stroke / 2;
            float offsetCornerRT = resolved.cornerRT + stroke / 2;
            float offsetCornerLT = resolved.cornerLT + stroke / 2;
            vertices[0] = RoundedVertex{{0.0f, 0.0f}, offsetPosition, offsetSize, strokeColor, {offsetCornerLB, offsetCornerRB, offsetCornerRT, offsetCornerLT}};
            vertices[1] = RoundedVertex{{1.0f, 0.0f}, offsetPosition, offsetSize, strokeColor, {offsetCornerLB, offsetCornerRB, offsetCornerRT, offsetCornerLT}};
            vertices[2] = RoundedVertex{{1.0f, 1.0f}, offsetPosition, offsetSize, strokeColor, {offsetCornerLB, offsetCornerRB, offsetCornerRT, offsetCornerLT}};
            vertices[3] = RoundedVertex{{0.0f, 1.0f}, offsetPosition, offsetSize, strokeColor, {offsetCornerLB, offsetCornerRB, offsetCornerRT, offsetCornerLT}};
        } else {
            vertices[0] = RoundedVertex{{0.0f, 0.0f}, position, size, strokeColor, {resolved.cornerLB, resolved.cornerRB, resolved.cornerRT, resolved.cornerLT}};
            vertices[1] = RoundedVertex{{1.0f, 0.0f}, position, size, strokeColor, {resolved.cornerLB, resolved.cornerRB, resolved.cornerRT, resolved.cornerLT}};
            vertices[2] = RoundedVertex{{1.0f, 1.0f}, position, size, strokeColor, {resolved.cornerLB, resolved.cornerRB, resolved.cornerRT, resolved.cornerLT}};
            vertices[3] = RoundedVertex{{0.0f, 1.0f}, position, size, strokeColor, {resolved.cornerLB, resolved.cornerRB, resolved.cornerRT, resolved.cornerLT}};
        }

        roundedBatchVertices.push_back(vertices[0]);
        roundedBatchVertices.push_back(vertices[1]);
        roundedBatchVertices.push_back(vertices[2]);
        roundedBatchVertices.push_back(vertices[3]);

        ++roundedCount;
    }

    { // Rect
        RoundedVertex vertices[4];

        if (strokeAlignment == UIStrokeAlignment::Outside) {
            vertices[0] = RoundedVertex{{0.0f, 0.0f}, position, size, color, {resolved.cornerLB, resolved.cornerRB, resolved.cornerRT, resolved.cornerLT}};
            vertices[1] = RoundedVertex{{1.0f, 0.0f}, position, size, color, {resolved.cornerLB, resolved.cornerRB, resolved.cornerRT, resolved.cornerLT}};
            vertices[2] = RoundedVertex{{1.0f, 1.0f}, position, size, color, {resolved.cornerLB, resolved.cornerRB, resolved.cornerRT, resolved.cornerLT}};
            vertices[3] = RoundedVertex{{0.0f, 1.0f}, position, size, color, {resolved.cornerLB, resolved.cornerRB, resolved.cornerRT, resolved.cornerLT}};
        } else if (strokeAlignment == UIStrokeAlignment::Middle) {
            Vector2 offsetPosition = position + Vector2(stroke / 2, stroke / 2);
            Vector2 offsetSize = size - Vector2(stroke, stroke);
            float offsetCornerLB = resolved.cornerLB - stroke / 2;
            float offsetCornerRB = resolved.cornerRB - stroke / 2;
            float offsetCornerRT = resolved.cornerRT - stroke / 2;
            float offsetCornerLT = resolved.cornerLT - stroke / 2;
        } else {
            Vector2 offsetPosition = position + Vector2(stroke, stroke);
            Vector2 offsetSize = size - Vector2(stroke * 2, stroke * 2);
            float offsetCornerLB = resolved.cornerLB - stroke;
            float offsetCornerRB = resolved.cornerRB - stroke;
            float offsetCornerRT = resolved.cornerRT - stroke;
            float offsetCornerLT = resolved.cornerLT - stroke;
            vertices[0] = RoundedVertex{{0.0f, 0.0f}, offsetPosition, offsetSize, color, {offsetCornerLB, offsetCornerRB, offsetCornerRT, offsetCornerLT}};
            vertices[1] = RoundedVertex{{1.0f, 0.0f}, offsetPosition, offsetSize, color, {offsetCornerLB, offsetCornerRB, offsetCornerRT, offsetCornerLT}};
            vertices[2] = RoundedVertex{{1.0f, 1.0f}, offsetPosition, offsetSize, color, {offsetCornerLB, offsetCornerRB, offsetCornerRT, offsetCornerLT}};
            vertices[3] = RoundedVertex{{0.0f, 1.0f}, offsetPosition, offsetSize, color, {offsetCornerLB, offsetCornerRB, offsetCornerRT, offsetCornerLT}};
        }

        roundedBatchVertices.push_back(vertices[0]);
        roundedBatchVertices.push_back(vertices[1]);
        roundedBatchVertices.push_back(vertices[2]);
        roundedBatchVertices.push_back(vertices[3]);

        ++roundedCount;
    }
}

const GLGlyph* RendererGL::getGlyph(const std::string& path, int size, Font* font, char c) {
    // Return glyph if it already exists
    auto& map = glyphs[path + "#" + std::to_string(size)];
    auto it = map.find(c);
    if (it != map.end()) return &it->second;
    
    const Glyph* g = font->getGlyph(c);
    if (!g) return nullptr;

    GLGlyph glyph;
    glyph.width = g->width;
    glyph.height = g->height;
    glyph.bearingX = g->bearingX;
    glyph.bearingY = g->bearingY;
    glyph.advance = g->advance;

    glyph.texture = GLTexture{};
    glyph.texture.bind();
    
    // Set texture wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Upload the bitmap data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, g->width, g->height, 0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.data());

    auto [inserted, _] = map.emplace(c, std::move(glyph));
    return &inserted->second;
}

void RendererGL::drawText(const UIDim2& position, const UIBounds& bounds, const std::string& text, const std::string& path, int size, const Color4& color) {
    if (quadCount > 0) flushQuadBatch();
    if (roundedCount > 0) flushRoundedBatch();

    size = static_cast<int>(std::round(size * currentScale));

    Font* font = FontManager::get().getFont(path, size);
    if (!font) {
        std::cout << "Failed to load font: " << path << std::endl;
        return;
    }

    Vector2 resolved = resolvePosition(position, bounds);
    float x = resolved.x;
    float y = resolved.y;

    for (char c : text) {
        const GLGlyph* g = getGlyph(path, size, font, c);
        if (!g) {
            std::cout << "Failed to get glyph for character: " << c << std::endl;
            continue;
        }

        if (g->width > 0 && g->height > 0) {
            // Flush text batch if we're at capacity
            if (textQuadCount >= maxTextQuads) {
                flushTextBatch();
            }

            // Calculate glyph position (fixed for proper orientation)
            float xpos = x + g->bearingX;
            float ypos = y + g->bearingY - g->height; // Fixed Y positioning

            float w = g->width;
            float h = g->height;

            // Create vertices for this glyph (fixed texture coordinates)
            TextVertex v0{{xpos, ypos}, {0.0f, 1.0f}, color};          // Bottom-left
            TextVertex v1{{xpos + w, ypos}, {1.0f, 1.0f}, color};      // Bottom-right  
            TextVertex v2{{xpos + w, ypos + h}, {1.0f, 0.0f}, color};  // Top-right
            TextVertex v3{{xpos, ypos + h}, {0.0f, 0.0f}, color};      // Top-left

            textBatchVertices.push_back(v0);
            textBatchVertices.push_back(v1);
            textBatchVertices.push_back(v2);
            textBatchVertices.push_back(v3);

            // For now, render each glyph immediately (not optimal but works)
            // Bind the glyph texture
            g->texture.bind();
            
            // Use shader and set texture uniform
            shaders.useShader("text");
            shaders.setUniformInt("text", "uTexture", 0); // Bind to texture unit 0
            
            // Flush just this glyph
            glBindVertexArray(textVAO);
            glBindBuffer(GL_ARRAY_BUFFER, textVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(TextVertex), &textBatchVertices[textBatchVertices.size() - 4]);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // Check for OpenGL errors
            GLenum error = glGetError();
            if (error != GL_NO_ERROR) {
                std::cout << "OpenGL error after drawing glyph: " << error << std::endl;
            }

            // Clear the last 4 vertices since we just rendered them
            textBatchVertices.erase(textBatchVertices.end() - 4, textBatchVertices.end());
        }

        // Advance the position for the next character
        x += g->advance;
    }
}

void RendererGL::endFrame() {
    flushQuadBatch();
    flushTextBatch();
    flushRoundedBatch();
    window->swapGLBuffers();
}