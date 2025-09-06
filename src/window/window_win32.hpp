/*
File:
    window_win32.hpp
Authors:
    Lucas
Purpose:
    Definition of the Window implementation interface for Win32
License:
    MIT (see LICENSE file)
*/

#pragma once

#include <string>
#include <iostream>

#include <windows.h>

#include <datatypes/vector2.hpp>

#include <window/window_impl.hpp>

class WindowWin32 : public WindowImpl {
private:
    HWND hwnd;
public:
    WindowWin32(const std::string& t, const Vector2& s);
    void update() override;
};