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
#include <optional>

#include <glad/wgl.h>

#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>

#include <datatypes/vector2.hpp>

#include <window/window_impl.hpp>
#include <app/app.hpp>

class WindowWin32 : public WindowImpl {
private:
    HWND hwnd;
    HDC hdc;

    HGLRC glContext;
    
    std::queue<WindowInput> inputs;
public:
    WindowWin32(const uint32_t& i, const WindowConfig& c);
    ~WindowWin32();
    std::queue<WindowInput> update() override;

    // OpenGL

    void bindGLContext() override;
    void makeGLCurrent() override;
    void swapGLBuffers() override;

    Vector2 getSize() override;

    void pushInput(WindowInput input);
};