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
#include <unordered_map>
#include <memory>
#include <iostream>
#include <queue>

#include <sol/sol.hpp>

#include <datatypes/vector2.hpp>
#include <window/window_input.hpp>

#include <window/window_impl.hpp>

#ifdef _WIN32
    #include "window_win32.hpp"
#endif

#include <render/renderer.hpp>

enum class WindowEvent {
    Update,
    Render,
    Input
};

class Window {
private:
    std::unique_ptr<WindowImpl> impl;
    std::queue<WindowInput> inputs;

    std::unordered_map<WindowEvent, std::vector<sol::function>> callbacks;

    bool blank = true;
    Vector2 lastSize;
public:
    Renderer renderer;

    Window(const uint32_t& id, const WindowConfig& config);
    void process();
    void update(float deltaTime);
    void render();
    void connectCallback(WindowEvent event, sol::function callback);
};