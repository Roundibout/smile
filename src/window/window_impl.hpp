#pragma once

#include <string>
#include <deque>
#include <optional>

#include <glad/gl.h>

#include "type/numbers.hpp"
#include "type/vector2.hpp"
#include "type/color4.hpp"
#include <window/window_input.hpp>
#include <render/renderer_types.hpp>

class App;

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
    const u32 id;
    WindowConfig config;

    RenderBackend rbackend;
    bool rbackendSet = false;
public:
    WindowImpl(App& app, const u32& i, const WindowConfig& c);
    virtual ~WindowImpl() = default;
    virtual std::deque<WindowInput> update() = 0;

    virtual void bindGLContext() = 0;
    virtual void makeGLCurrent() = 0;
    virtual void swapGLBuffers() = 0;

    const u32 getId();
    virtual Vector2 getSize() = 0;
    virtual Vector2 getPosition() = 0;

    virtual bool isKeyDown(KeyCode key) = 0;

    virtual void setMousePosition(const Vector2& position) = 0;

    virtual void freezeMouse() = 0;
    virtual void unfreezeMouse() = 0;

    virtual void setCursor(Cursor cursor) = 0;
    virtual void hideCursor() = 0;
    virtual void showCursor() = 0;

    virtual void setCapture() = 0;
    virtual void releaseCapture() = 0;

    WindowConfig getConfig();

    App& app; // TODO: clean up the win32 stuff so we don't have to do stuff like this
};