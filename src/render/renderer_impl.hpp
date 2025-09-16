/*
File:
    renderer_impl.hpp
Authors:
    Lucas
Purpose:
    Definition of the Renderer implementation class
License:
    MIT (see LICENSE file)
*/

#pragma once

#include <datatypes/vector2.hpp>
#include <datatypes/color4.hpp>

#include <window/window_impl.hpp>

class RendererImpl {
protected:
    WindowImpl* window;
public:
    RendererImpl(WindowImpl* w);

    virtual void beginFrame() = 0;

    virtual void drawRect(const Vector2& position, const Vector2& size, const Color4& color) = 0;
    virtual void drawText(const std::string text, const Vector2& position, const std::string path, int size) = 0;

    virtual void endFrame() = 0;
};