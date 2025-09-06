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
#include <vector>
#include <memory>
#include <iostream>

#include <sol/sol.hpp>

#include <datatypes/vector2.hpp>

#include "window/window_impl.hpp"
#ifdef _WIN32
    #include "window/window_win32.hpp"
#endif

enum class WindowEvent {
    Update,
    Render
};

class Window {
private:
    std::unique_ptr<WindowImpl> impl;
    std::unordered_map<WindowEvent, std::vector<sol::function>> callbacks;
public:
    Window(const std::string& title, const Vector2& size);
    void update(float deltaTime);
    void connectCallback(WindowEvent event, sol::function callback);
};