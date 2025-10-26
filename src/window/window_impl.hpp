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
#include <deque>
#include <optional>

#include <glad/gl.h>

#include <datatypes/vector2.hpp>
#include <window/window_input.hpp>
#include <render/renderer_types.hpp>

struct WindowConfig {
    std::string title = "Window";
    Vector2 size = Vector2(800, 600);

    Color4 color = Color4(255, 255, 255);

    bool resizable = true;
    std::optional<Vector2> maxSize;
    Vector2 minSize = Vector2(400, 200);

    bool maximized = false;
    bool minimized = false;

    bool maximizable = true;
    bool minimizable = true;
};

enum class Cursor {
    Arrow,
    HorizontalResize,
    VerticalResize
};

class WindowImpl {
protected:
    const uint32_t id;
    WindowConfig config;

    RenderBackend rbackend;
    bool rbackendSet = false;
public:
    WindowImpl(const uint32_t& i, const WindowConfig& c);
    virtual ~WindowImpl() = default;
    virtual std::deque<WindowInput> update() = 0;

    virtual void bindGLContext() = 0;
    virtual void makeGLCurrent() = 0;
    virtual void swapGLBuffers() = 0;

    const uint32_t getId();
    virtual Vector2 getSize() = 0;
    virtual Vector2 getPosition() = 0;

    virtual bool isKeyDown(KeyCode key) = 0;

    virtual void setMousePosition(const Vector2& position) = 0;

    virtual void freezeMouse() = 0;
    virtual void unfreezeMouse() = 0;

    virtual void setCursor(Cursor cursor) = 0;

    virtual void setCapture() = 0;
    virtual void releaseCapture() = 0;

    WindowConfig getConfig();
};