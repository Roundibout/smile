/*
File:
    window.hpp
Authors:
    Lucas
Purpose:
    Definition of the Window class, used to create windows in Smile
License:
    MIT (see LICENSE file)
*/

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <deque>
#include <functional>

#include <datatypes/vector2.hpp>
#include <window/window_input.hpp>

#include <window/window_impl.hpp>

#ifdef _WIN32
    #include "window_win32.hpp"
#endif

#include <render/renderer.hpp>

#include <ui/widgets/widget.hpp>

class App;

enum class WindowEvent {
    Update,
    Render,
    Input
};

class Window {
private:
    App& app;

    std::unique_ptr<WindowImpl> impl;
    std::deque<WindowInput> inputs;

    std::vector<std::unique_ptr<Widget>> widgets;

    std::vector<std::function<void(float deltaTime, const UIBounds& bounds)>> updateCallbacks;
    std::vector<std::function<void(const UIBounds& bounds)>> renderCallbacks;
    std::vector<std::function<void(WindowInput& input, const UIBounds& bounds)>> inputCallbacks;

    bool blank = true;
    Vector2 lastSize;
public:
    Renderer renderer;

    Window(App& app, const uint32_t& id, const WindowConfig& config);

    void process();
    void update(float deltaTime);
    void render(float uiScale);

    void connectUpdate(std::function<void(float deltaTime, const UIBounds& bounds)> callback);
    void connectRender(std::function<void(const UIBounds& bounds)> callback);
    void connectInput(std::function<void(WindowInput& input, const UIBounds& bounds)> callback);

    bool isKeyDown(KeyCode key);

    void setMousePosition(const Vector2& position);

    void freezeMouse();
    void unfreezeMouse();

    void setCursor(Cursor cursor);
    void hideCursor();
    void showCursor();

    void setCapture();
    void releaseCapture();

    template <typename T, typename... Args>
    T* addWidget(Args&&... args) {
        static_assert(std::is_base_of<Widget, T>::value, "T must be a Widget");

        auto widget = std::make_unique<T>(app, this, std::forward<Args>(args)...);
        T* ptr = widget.get(); // Keep raw pointer for return
        widgets.push_back(std::move(widget));

        return ptr;
    }
};