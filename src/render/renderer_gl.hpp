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

#include <glad/gl.h>

#include <render/renderer_impl.hpp>
#include <render/shader_manager.hpp>

class RendererGL : public RendererImpl {
private:
    ShaderManagerGL shaders;

    GLuint batchVAO, batchVBO, batchEBO;
    std::vector<Vertex> batchVertices;
    std::vector<unsigned int> batchIndices;

    size_t quadCount = 0;
    const size_t maxQuads = 1000;

    void flushBatch();
public:
    RendererGL(WindowImpl* w);

    void beginFrame() override;

    void drawRect(const Vector2& position, const Vector2& size, const Color4& color) override;

    void endFrame() override;
};