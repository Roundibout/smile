/*
File:
    window_impl.hpp
Authors:
    Lucas
Purpose:
    Definition of the common Window implementation interface
License:
    MIT (see LICENSE file)
*/

#pragma once

#include <string>
#include <queue>

#include <glad/gl.h>

#include <datatypes/vector2.hpp>
#include <window/window_input.hpp>
#include <render/renderer_types.hpp>

class WindowImpl {
protected:
    const uint32_t id;
    std::string title;
    Vector2 size;

    RenderBackend rbackend;
    bool rbackendSet = false;
public:
    WindowImpl(const uint32_t& i, const std::string& t, const Vector2& s);
    virtual std::queue<WindowInput> update() = 0;

    virtual void bindGLContext() = 0;
    virtual void makeGLCurrent() = 0;
    virtual void swapGLBuffers() = 0;

    const uint32_t getId();
    virtual Vector2 getSize() = 0;
};