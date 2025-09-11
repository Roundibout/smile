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

#include <window/window_impl.hpp>

class RendererImpl {
protected:
    WindowImpl* window;
public:
    RendererImpl(WindowImpl* w);

    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
};