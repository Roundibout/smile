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

#include <glad/gl.h>

#include <render/renderer_impl.hpp>

class RendererGL : public RendererImpl {
private:
    
public:
    RendererGL(WindowImpl* w);
    void beginFrame() override;
    void endFrame() override;
};