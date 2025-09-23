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
#include <string>

#include <window/window_impl.hpp>
#include <render/renderer_impl.hpp>
#include <render/renderer_gl.hpp>

class Renderer {
private:
    std::unique_ptr<RendererImpl> impl;
    bool dirtied = false;
public:
    Renderer(WindowImpl* window);

    void beginFrame();

    void drawRect(const Vector2& position, const Vector2& size, const Color4& color);
    void drawText(const std::string& text, const Vector2& position, const std::string& path, int size, const Color4& color);

    void endFrame();

    void dirty();
    void clean();
    bool isDirty();
};