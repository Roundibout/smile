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

#include <glad/gl.h>

#include <render/renderer_impl.hpp>
#include <render/shader_manager.hpp>
#include <render/font_manager.hpp>

struct Vertex {
    Vector2 position;
    Color4 color;
};

struct TextVertex {
    Vector2 position;
    Vector2 texCoord;
    Color4 color;
};

struct GLGlyph {
    GLuint textureID;
    int width, height;
    int bearingX, bearingY;
    int advance;
};

class RendererGL : public RendererImpl {
private:
    ShaderManagerGL shaders;

    // Quad
    GLuint quadVAO, quadVBO, quadEBO;
    size_t quadCount = 0;
    const size_t maxQuads = 1000;

    // Text
    GLuint textVAO, textVBO, textEBO;
    size_t textQuadCount = 0;
    const size_t maxTextQuads = 1000;
    std::unordered_map<std::string, std::unordered_map<char, GLGlyph>> glyphs;

    // Batch data
    std::vector<Vertex> batchVertices;
    std::vector<unsigned int> batchIndices;
    std::vector<TextVertex> textBatchVertices;
    std::vector<unsigned int> textBatchIndices;

    std::array<float, 16> currentProjection;

    void flushQuadBatch();
    void flushTextBatch();

    const GLGlyph* getGlyph(const std::string& path, int size, Font* font, char c);
public:
    RendererGL(WindowImpl* w);

    void beginFrame() override;

    void drawRect(const Vector2& position, const Vector2& size, const Color4& color) override;
    void drawText(const std::string text, const Vector2& position, const std::string path, int size, const Color4& color) override;

    void endFrame() override;
};