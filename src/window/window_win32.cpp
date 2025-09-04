/*
File:
    window_win32.cpp
Authors:
    Lucas
Purpose:
    Implementation of the Window implementation class for Win32
License:
    MIT (see LICENSE file)
*/

#include "window_win32.hpp"

// A value for if the window class already registered, used in the function registerWindowClass
static bool classRegistered = false;

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

// Register the window class if it does not exist already
static void registerWindowClass(HINSTANCE hInstance) {
    if (classRegistered) return;
    
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SmileWindow";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClass(&wc);
    classRegistered = true;
}

// Win32 window constructor
WindowWin32::WindowWin32(const std::string& t, const Vector2& s) : WindowImpl(t, s) {
    std::cout << "Creating win32 window" << std::endl; // temp

    // Get the handle to Smile
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    // Register a window class with Windows if one has not been registered yet
    registerWindowClass(hInstance);

    // Create a window associated with Smile
    hwnd = CreateWindow(
        "SmileWindow",
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        size.x, size.y,
        nullptr, nullptr, hInstance, nullptr
    );

    // Display the window
    ShowWindow(hwnd, SW_SHOW);
}