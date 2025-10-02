/*
File:
    renderer_gl.hpp
Authors:
    Lucas
Purpose:
    Definition of the Renderer implementation class for OpenGL
License:
    MIT (see LICENSE file)
*/

#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>

#include <glad/gl.h>

#include <render/renderer_impl.hpp>
#include <render/shader_manager.hpp>
#include <render/font_manager.hpp>

struct TriangleVertex {
    Vector2 position;
    Color4 color;
};

struct TextVertex {
    Vector2 position;
    Vector2 texCoord;
    Color4 color;
    float texID;
};

struct RoundedVertex {
    Vector2 position;
    Vector2 rectPosition;
    Vector2 rectSize;
    Color4 color;
    float corners[4];
};

class GLTexture {
private:
    GLuint id = 0;
public:
    GLTexture() {
        glGenTextures(1, &id);
    }
    ~GLTexture() {
        if (id != 0) {
            glDeleteTextures(1, &id);
        }
    }

    GLTexture(const GLTexture&) = delete;
    GLTexture& operator=(const GLTexture&) = delete;

    GLTexture(GLTexture&& other) noexcept : id(other.id) {
        other.id = 0;
    }
    GLTexture& operator=(GLTexture&& other) noexcept {
        if (this != &other) {
            if (id != 0) glDeleteTextures(1, &id);
            id = other.id;
            other.id = 0;
        }
        return *this;
    }

    GLuint get() const {return id;}
    void bind(GLenum target = GL_TEXTURE_2D) const {
        glBindTexture(target, id);
    }
};

struct GLGlyph {
    GLTexture texture;
    int width, height = 0;
    int bearingX, bearingY = 0;
    int advance = 0;

    GLGlyph() = default;

    GLGlyph(const GLGlyph&) = delete;
    GLGlyph& operator=(const GLGlyph&) = delete;

    GLGlyph(GLGlyph&&) noexcept = default;
    GLGlyph& operator=(GLGlyph&&) noexcept = default;
};

class RendererGL : public RendererImpl {
private:
    ShaderManagerGL shaders;

    // Quad
    GLuint quadVAO, quadVBO, quadEBO;
    size_t quadCount = 0;
    const size_t maxQuads = 1000;

    // Rounded
    GLuint roundedVAO, roundedVBO, roundedEBO;
    size_t roundedCount = 0;
    const size_t maxRoundeds = 1000;

    // Text
    GLuint textVAO, textVBO, textEBO;
    size_t textQuadCount = 0;
    const size_t maxTextQuads = 1000;
    std::unordered_map<std::string, std::unordered_map<char, GLGlyph>> glyphs;

    // Batch data
    std::vector<TriangleVertex> batchVertices;
    std::vector<unsigned int> batchIndices;
    std::vector<RoundedVertex> roundedBatchVertices;
    std::vector<unsigned int> roundedBatchIndices;
    std::vector<TextVertex> textBatchVertices;
    std::vector<unsigned int> textBatchIndices;

    std::array<float, 16> currentProjection;

    void flushQuadBatch();
    void flushRoundedBatch();
    void flushTextBatch();

    const GLGlyph* getGlyph(const std::string& path, int size, Font* font, char c);
public:
    RendererGL(WindowImpl* w);

    void beginFrame(float scale) override;

    void drawRect(const UILayout& layout, const UIBounds& bounds, const Color4& color) override;
    void drawStrokeRect(const UILayout& layout, const UIBounds& bounds, const Color4& color, int stroke, const Color4& strokeColor, const UIStrokeAlignment& strokeAlignment) override;
    void drawRoundedRect(const UILayout& layout, const UIBounds& bounds, const Color4& color) override;
    void drawRoundedStrokeRect(const UILayout& layout, const UIBounds& bounds, const Color4& color, int stroke, const Color4& strokeColor, const UIStrokeAlignment& strokeAlignment) override;
    void drawText(const UIDim2& position, const UIBounds& bounds, const std::string& text, const std::string& path, int size, const Color4& color) override;

    void endFrame() override;
};