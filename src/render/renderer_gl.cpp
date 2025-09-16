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
    
    // Create VAO/VBO/EBO for batching
    glGenVertexArrays(1, &batchVAO);
    glGenBuffers(1, &batchVBO);
    glGenBuffers(1, &batchEBO);

    glBindVertexArray(batchVAO);

    // Allocate buffer for vertices (dynamic draw for live updates which is important)
    glBindBuffer(GL_ARRAY_BUFFER, batchVBO);
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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, batchIndices.size() * sizeof(unsigned int), batchIndices.data(), GL_STATIC_DRAW);

    // Set up vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void RendererGL::flushBatch() {
    if (quadCount == 0) return;

    glBindVertexArray(batchVAO);
    glBindBuffer(GL_ARRAY_BUFFER, batchVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, batchVertices.size() * sizeof(Vertex), batchVertices.data());

    shaders.useShader("quad");
    glDrawElements(GL_TRIANGLES, quadCount * 6, GL_UNSIGNED_INT, 0);

    batchVertices.clear();
    quadCount = 0;

    glBindVertexArray(0);
}

void RendererGL::beginFrame() {
    window->makeGLCurrent();

    // Get and set viewport size
    Vector2 size = window->getSize();
    glViewport(0, 0, int(size.x), int(size.y));

    // Get orthographic projection
    auto proj = makeOrtho(0.0f, size.x, size.y, 0.0f);

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Clear the screen
    Color4 color = window->getConfig().color;
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    batchVertices.clear();
    quadCount = 0;

    // Set uniforms
    shaders.setUniformMat4("quad", "uProjection", proj.data());
}

void RendererGL::drawRect(const Vector2& position, const Vector2& size, const Color4& color) {
    if (quadCount >= maxQuads) flushBatch();

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

void RendererGL::drawText(const std::string text, const Vector2& position, const std::string path, int size) {
    // do stuff here
}

void RendererGL::endFrame() {
    flushBatch();
    window->swapGLBuffers();
}