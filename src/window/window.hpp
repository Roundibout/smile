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
#include <memory>
#include <iostream>

#include <datatypes/vector2.hpp>

#include "window/window_impl.hpp"
#ifdef _WIN32
    #include "window/window_win32.hpp"
#endif

class Window {
private:
    std::unique_ptr<WindowImpl> impl;
public:
    Window(const std::string& title, const Vector2& size);
};