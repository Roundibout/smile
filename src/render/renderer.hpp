/*
File:
    renderer.hpp
Authors:
    Lucas
Purpose:
    Definition of the Renderer class
License:
    MIT (see LICENSE file)
*/

#pragma once

#include <memory>

#include <window/window_impl.hpp>
#include <render/renderer_impl.hpp>
#include <render/renderer_gl.hpp>

class Renderer {
private:
    std::unique_ptr<RendererImpl> impl;
public:
    Renderer(WindowImpl* window);
    void beginFrame();
    void endFrame();
};