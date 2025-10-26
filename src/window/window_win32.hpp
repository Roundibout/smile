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
#include <optional>
#include <algorithm>
#include <unordered_map>
#include <deque>
#include <utility>

#include <glad/wgl.h>

#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>

#include <winres.h>

#include <datatypes/vector2.hpp>

#include <window/window_impl.hpp>
#include <app/app.hpp>

#include <util/logger.hpp>

class WindowWin32 : public WindowImpl {
private:
    HWND hwnd;
    HDC hdc;

    HGLRC glContext;
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
    
    std::deque<WindowInput> inputs;

    LPCSTR cursorId = IDC_ARROW;
public:
    WindowWin32(const uint32_t& i, const WindowConfig& c);
    ~WindowWin32();
    std::deque<WindowInput> update() override;

    // OpenGL

    void bindGLContext() override;
    void makeGLCurrent() override;
    void swapGLBuffers() override;

    Vector2 getSize() override;
    Vector2 getPosition() override;

    bool isKeyDown(KeyCode key) override;

    void setMousePosition(const Vector2& position) override;

    void setCursor(Cursor cursor) override;

    void freezeMouse() override;
    void unfreezeMouse() override;

    void setCapture() override;
    void releaseCapture() override;
    
    void pushInput(WindowInput input);

    // Should be private but WndProc

    Vector2 lastMousePos;
    bool hasLastMousePos = false;

    Vector2 mouseGoal;
    bool reachingMouseGoal = false;
};