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
#include <core/app.hpp>

// A value for if the window class already registered, used in the function registerWindowClass
static bool classRegistered = false;

void pushInput(HWND hwnd, WindowInput input) {
    WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window

    // Push the input to the queue
    window->pushInput(input);
}

static const std::pair<WPARAM, KeyCode> keyPairs[] = {
    {'Q', KeyCode::Q}, {'W', KeyCode::W}, { 'E', KeyCode::E},
    {'R', KeyCode::R}, {'T', KeyCode::T}, { 'Y', KeyCode::Y},
    {'U', KeyCode::U}, {'I', KeyCode::I}, { 'O', KeyCode::O},
    {'P', KeyCode::P}, {'A', KeyCode::A}, { 'S', KeyCode::S},
    {'D', KeyCode::D}, {'F', KeyCode::F}, { 'G', KeyCode::G},
    {'H', KeyCode::H}, {'J', KeyCode::J}, { 'K', KeyCode::K},
    {'L', KeyCode::L}, {'Z', KeyCode::Z}, { 'X', KeyCode::X},
    {'C', KeyCode::C}, {'V', KeyCode::V}, { 'B', KeyCode::B},
    {'N', KeyCode::N}, {'M', KeyCode::M},

    {'1', KeyCode::One}, {'2', KeyCode::Two}, {'3', KeyCode::Three},
    {'4', KeyCode::Four}, {'5', KeyCode::Five}, {'6', KeyCode::Six},
    {'7', KeyCode::Seven}, {'8', KeyCode::Eight}, {'9', KeyCode::Nine},
    {'0', KeyCode::Zero},

    {VK_SPACE, KeyCode::Space},
    {VK_OEM_3, KeyCode::BackQuote}, {VK_OEM_MINUS, KeyCode::Minus},
    {VK_OEM_PLUS, KeyCode::Equals}, {VK_OEM_4, KeyCode::LeftSquareBracket},
    {VK_OEM_6, KeyCode::RightSquareBracket}, {VK_OEM_5, KeyCode::BackSlash},
    {VK_OEM_1, KeyCode::Semicolon}, {VK_OEM_7, KeyCode::SingleQuote},
    {VK_OEM_COMMA, KeyCode::Comma}, {VK_OEM_PERIOD, KeyCode::Period},
    {VK_OEM_2, KeyCode::Slash},

    {VK_ESCAPE, KeyCode::Escape}, {VK_F1, KeyCode::F1},
    {VK_F2, KeyCode::F2}, {VK_F3, KeyCode::F3}, {VK_F4, KeyCode::F4},
    {VK_F5, KeyCode::F5}, {VK_F6, KeyCode::F6}, {VK_F7, KeyCode::F7},
    {VK_F8, KeyCode::F8}, {VK_F9, KeyCode::F9}, {VK_F10, KeyCode::F10},
    {VK_F11, KeyCode::F11}, {VK_F12, KeyCode::F12},
    {VK_DELETE, KeyCode::Delete},

    {VK_TAB, KeyCode::Tab}, {VK_CAPITAL, KeyCode::CapsLock},
    {VK_SHIFT, KeyCode::Shift}, {VK_CONTROL, KeyCode::Control},
    {VK_LWIN, KeyCode::Super}, {VK_RWIN, KeyCode::Super},
    {VK_MENU, KeyCode::Alt}, {VK_RETURN, KeyCode::Enter},
    {VK_BACK, KeyCode::Back},

    {VK_LEFT, KeyCode::Left}, {VK_RIGHT, KeyCode::Right},
    {VK_UP, KeyCode::Up}, {VK_DOWN, KeyCode::Down},
    {VK_HOME, KeyCode::Home}, {VK_END, KeyCode::End},
    {VK_PRIOR, KeyCode::PageUp}, {VK_NEXT, KeyCode::PageDown},
};

static std::unordered_map<WPARAM, KeyCode> vkToKey;
static std::unordered_map<KeyCode, WPARAM> keyToVk;

static void initKeyMaps() {
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    for (auto [vk, key] : keyPairs) {
        vkToKey[vk] = key;
        keyToVk[key] = vk;
    }
}

KeyCode vkToKeyCode(WPARAM vk) {
    initKeyMaps();
    auto it = vkToKey.find(vk);
    return (it != vkToKey.end()) ? it->second : KeyCode::Unknown;
}

WPARAM keyCodeToVk(KeyCode key) {
    initKeyMaps();
    auto it = keyToVk.find(key);
    return (it != keyToVk.end()) ? it->second : 0;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // TODO: get the window here instead of in each case
    switch (msg) { // Each case has its own scope so the variables don't overlap
        case WM_NCCREATE: {
            // Set the userdata for the window (the window class we create) so we can use it in other cases
            CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
            WindowWin32* window = reinterpret_cast<WindowWin32*>(cs->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
            return DefWindowProc(hwnd, msg, wParam, lParam); // Do your thing Windows...
        }
        case WM_DESTROY: {
            KillTimer(hwnd, 1);
            WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window
            window->app.destroyWindow(window->getId());
            PostQuitMessage(0);
            return 0;
        }

        // FIX FOR STALLING DURING RESIZE AND MOVE

        case WM_SIZE: {
            WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window
            window->app.step();
            return 0;
        }
        case WM_MOVE: {
            WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window
            window->app.step();
            return 0;
        }
        case WM_EXITSIZEMOVE: {
            WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window

            WindowInput sizeInput;
            sizeInput.type = WindowInputType::WindowResized;
            sizeInput.window.size = window->getSize();

            pushInput(hwnd, sizeInput);

            WindowInput positionInput;
            positionInput.type = WindowInputType::WindowResized;
            positionInput.window.position = window->getPosition();

            pushInput(hwnd, positionInput);
            return 0;
        }

        // ENFORCE MINIMUM AND MAXIMUM SIZE
        case WM_GETMINMAXINFO: {
            WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window
            if (!window) {
                // Window not fully created yet, ignore this message
                return 0;
            }

            MINMAXINFO* mmi = reinterpret_cast<MINMAXINFO*>(lParam);
            WindowConfig config = window->getConfig();

            // Always set the minimum size
            mmi->ptMinTrackSize.x = static_cast<LONG>(config.minSize.x);
            mmi->ptMinTrackSize.y = static_cast<LONG>(config.minSize.y);

            // Only set max if present
            if (config.maxSize.has_value()) {
                mmi->ptMaxTrackSize.x = static_cast<LONG>(config.maxSize->x);
                mmi->ptMaxTrackSize.y = static_cast<LONG>(config.maxSize->y);
            }
            return 0;
        }

        // MOUSE
        case WM_MOUSEMOVE: {
            WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window

            int x = GET_X_LPARAM(lParam);
            int y = int(window->getSize().y) - GET_Y_LPARAM(lParam);
            Vector2 currentPos(x, y);

            if (window->reachingMouseGoal) { // I do not like this scuffed fix. There has to be a better way!
                float dx = currentPos.x - window->mouseGoal.x;
                float dy = currentPos.y - window->mouseGoal.y;

                if (sqrtf(dx*dx + dy*dy) <= 10) {
                    window->reachingMouseGoal = false;
                } else {
                    // Try to set the position again because Win32
                    RECT rect;
                    GetClientRect(hwnd, &rect);
                    POINT clientPoint = {static_cast<int>(window->mouseGoal.x), static_cast<int>(rect.bottom - window->mouseGoal.y)};
                    ClientToScreen(hwnd, &clientPoint);

                    SetCursorPos(clientPoint.x, clientPoint.y);

                    return 0;
                }
            }

            Vector2 delta(0, 0);
            if (window->hasLastMousePos) {
                delta = currentPos - window->lastMousePos;
            }

            window->lastMousePos = currentPos;
            window->hasLastMousePos = true;

            WindowInput input;
            input.type = WindowInputType::MouseMove;
            input.mouse.position = currentPos;
            input.mouse.delta = delta; // store delta in your struct

            pushInput(hwnd, input);
            return 0;
        }
        case WM_LBUTTONDOWN: {
            WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window

            WindowInput input;
            input.type = WindowInputType::MouseButtonDown;
            input.mouse.button = MouseButton::Left;
            input.mouse.position = Vector2(GET_X_LPARAM(lParam), int(window->getSize().y) - GET_Y_LPARAM(lParam));

            pushInput(hwnd, input);
            return 0;
        }
        case WM_LBUTTONUP: {
            WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window

            WindowInput input;
            input.type = WindowInputType::MouseButtonUp;
            input.mouse.button = MouseButton::Left;
            input.mouse.position = Vector2(GET_X_LPARAM(lParam), int(window->getSize().y) - GET_Y_LPARAM(lParam));

            pushInput(hwnd, input);
            return 0;
        }
        case WM_RBUTTONDOWN: {
            WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window

            WindowInput input;
            input.type = WindowInputType::MouseButtonDown;
            input.mouse.button = MouseButton::Right;
            input.mouse.position = Vector2(GET_X_LPARAM(lParam), int(window->getSize().y) - GET_Y_LPARAM(lParam));

            pushInput(hwnd, input);
            return 0;
        }
        case WM_RBUTTONUP: {
            WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window

            WindowInput input;
            input.type = WindowInputType::MouseButtonUp;
            input.mouse.button = MouseButton::Right;
            input.mouse.position = Vector2(GET_X_LPARAM(lParam), int(window->getSize().y) - GET_Y_LPARAM(lParam));

            pushInput(hwnd, input);
            return 0;
        }
        case WM_MBUTTONDOWN: {
            WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window

            WindowInput input;
            input.type = WindowInputType::MouseButtonDown;
            input.mouse.button = MouseButton::Middle;
            input.mouse.position = Vector2(GET_X_LPARAM(lParam), int(window->getSize().y) - GET_Y_LPARAM(lParam));

            pushInput(hwnd, input);
            return 0;
        }
        case WM_MBUTTONUP: {
            WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window

            WindowInput input;
            input.type = WindowInputType::MouseButtonUp;
            input.mouse.button = MouseButton::Middle;
            input.mouse.position = Vector2(GET_X_LPARAM(lParam), int(window->getSize().y) - GET_Y_LPARAM(lParam));

            pushInput(hwnd, input);
            return 0;
        }
        case WM_MOUSEWHEEL: {
            WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window

            WindowInput input;
            input.type = WindowInputType::MouseScroll;
            
            input.mouse.position = window->lastMousePos; // Dumb

            input.mouse.scroll = Vector2(0.0f, float(GET_WHEEL_DELTA_WPARAM(wParam)) / 120.0f);

            pushInput(hwnd, input);
            return 0;
        }
        case WM_MOUSEHWHEEL: {
            WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window
            
            WindowInput input;
            input.type = WindowInputType::MouseScroll;
            
            POINT point;
            point.x = GET_X_LPARAM(lParam);
            point.y = GET_Y_LPARAM(lParam);
            ScreenToClient(hwnd, &point); // convert to window-relative space because its in display-relative space for some reason
            input.mouse.position = Vector2(float(point.x), window->getSize().y - float(GET_Y_LPARAM(lParam))); // convert from weird Windows types to real values

            input.mouse.scroll = Vector2(float(GET_WHEEL_DELTA_WPARAM(wParam)) / 120.0f, 0.0f);

            pushInput(hwnd, input);
            return 0;
        }
        // KEYBOARD
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN: { // KEYDOWN + SYSKEYDOWN, gets when any key is pressed
            if (lParam & (1 << 30)) { // Prevent repeated keydowns
                return 0;
            }

            WindowInput input;
            input.type = WindowInputType::KeyDown;
            input.key = vkToKeyCode(wParam);

            pushInput(hwnd, input);
            return 0;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP: { // KEYUP + SYSKEYUP, gets when any key is lifted
            WindowInput input;
            input.type = WindowInputType::KeyUp;
            input.key = vkToKeyCode(wParam);

            pushInput(hwnd, input);
            return 0;
        }
        default: {
            // Do default
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }

    // moment before cooked (fallback)
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// Register the window class if it does not exist already
static void registerWindowClass(HINSTANCE hInstance) {
    if (classRegistered) return;
    
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SmileWindow";
    wc.style = CS_OWNDC;
    wc.hbrBackground = nullptr;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(APP_ICON));
    wc.hIconSm = wc.hIcon;

    RegisterClassEx(&wc);
    classRegistered = true;
}

// Win32 window constructor
WindowWin32::WindowWin32(App& app, const uint32_t& i, const WindowConfig& c) : WindowImpl(app, i, c) {
    SetProcessDPIAware(); // Set this so it isn't blurry

    // Get the handle to Smile
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    // Register a window class with Windows if one has not been registered yet
    registerWindowClass(hInstance);

    // Create window style from config
    DWORD style = WS_OVERLAPPEDWINDOW;

    if (!config.resizable) style &= ~WS_SIZEBOX;
    if (!config.maximizable) style &= ~WS_MAXIMIZEBOX;
    if (!config.minimizable) style &= ~WS_MINIMIZEBOX;
    
    // Create a window associated with Smile
    hwnd = CreateWindow(
        "SmileWindow",
        config.title.c_str(),
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        static_cast<int>(config.size.x), static_cast<int>(config.size.y),
        nullptr, nullptr, hInstance, this // pass in the window so we can use it in the window procedure
    );

    // Set caption color
    COLORREF captionColor = RGB(
        std::round(std::clamp(config.color.r * 255.0f, 0.0f, 255.0f)), 
        std::round(std::clamp(config.color.g * 255.0f, 0.0f, 255.0f)), 
        std::round(std::clamp(config.color.b * 255.0f, 0.0f, 255.0f))
    );
    DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &captionColor, sizeof(captionColor));

    // Get display context for use in rendering
    hdc = GetDC(hwnd);

    // Display the window
    if (config.maximizable == true && config.maximized == true) {
        ShowWindow(hwnd, SW_MAXIMIZE);
    } else if (config.minimizable == true && config.minimized == true) {
        ShowWindow(hwnd, SW_MINIMIZE);
    } else {
        ShowWindow(hwnd, SW_SHOW);
    }

    // Update the window
    UpdateWindow(hwnd);
}

std::deque<WindowInput> WindowWin32::update() {
    MSG msg;
    // Process all pending messages
    while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Merge all mouse move events into one
    WindowInput merged{};
    bool haveMerged = false;
    
    for (auto it = inputs.begin(); it != inputs.end();) {
        if (it->type == WindowInputType::MouseMove) {
            if (!haveMerged) {
                merged = *it;
                haveMerged = true;
            } else {
                merged.mouse.delta += it->mouse.delta;
                merged.mouse.position = it->mouse.position;
            }
            it = inputs.erase(it);
        } else {
            ++it;
        }
    }
    
    if (haveMerged) {
        inputs.push_back(merged);
    }

    // Copy inputs to a new queue for use outside
    std::deque<WindowInput> inputsCopy;
    std::swap(inputsCopy, inputs);

    return inputsCopy;
}

void WindowWin32::bindGLContext() {
    if (rbackendSet) {
        Logger::print("Render context already bound");
        return;
    }
    
    // Set variables for the backend used and that it is set
    rbackend = RenderBackend::GL;
    rbackendSet = true;

    HINSTANCE hInstance = GetModuleHandle(nullptr);

    // Create a dummy
    HWND dummyHwnd = CreateWindow(
        "SmileWindow",
        "Dummy",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1, 1,
        nullptr, nullptr, hInstance, nullptr
    );

    ShowWindow(dummyHwnd, SW_HIDE);

    // Get dummy display context
    HDC dummyHdc = GetDC(dummyHwnd);

    // Create a PIXEL FORMAT DESCRIPTOR
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int tempPF = ChoosePixelFormat(dummyHdc, &pfd);
    SetPixelFormat(dummyHdc, tempPF, &pfd);

    // Create dummy context (we need it to move out of caveman GL)
    HGLRC dummyCtx = wglCreateContext(dummyHdc);
    wglMakeCurrent(dummyHdc, dummyCtx); // Make it current so we can use it

    // Load WGL extensions (needed for core context creation)
    if (!gladLoaderLoadWGL(dummyHdc)) {
        Logger::error("Failed to load WGL extensions");
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(dummyCtx);
        DestroyWindow(dummyHwnd);
        return;
    }

    // Create pixel format attributes
    int pixelFormatAttribs[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        WGL_SAMPLE_BUFFERS_ARB, GL_TRUE, // Request MSAA buffer
        WGL_SAMPLES_ARB, 4, // 4x MSAA
        0
    };

    int pf;
    UINT numFormats;
    if (!wglChoosePixelFormatARB(hdc, pixelFormatAttribs, nullptr, 1, &pf, &numFormats) || numFormats == 0) {
        Logger::warn("MSAA pixel format not supported, falling back to standard format");
        pf = ChoosePixelFormat(hdc, &pfd);
    }

    SetPixelFormat(hdc, pf, &pfd);

    // Attributes list for modern GL (4.6)
    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    // Create modern GL context
    glContext = wglCreateContextAttribsARB(hdc, 0, attribs);

    // Delete the dummy
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(dummyCtx);
    wglMakeCurrent(hdc, glContext); // Make it current so we can use modern GL

    // Load GL from glad
    if (!gladLoaderLoadGL()) {
        Logger::error("Failed to load OpenGL");
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(glContext);
    }

    // Enable VSync
    wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
    wglSwapIntervalEXT(1);
}

void WindowWin32::makeGLCurrent() {
    if (rbackendSet && rbackend == RenderBackend::GL) {
        // Just make the context current
        wglMakeCurrent(hdc, glContext);

        // Enable VSync
        wglSwapIntervalEXT(1);
    } else {
        Logger::error("Render context either does not exist or is not OpenGL");
    }
}

void WindowWin32::swapGLBuffers() {
    if (rbackendSet && rbackend == RenderBackend::GL) {
        // Just swap buffers
        SwapBuffers(hdc);
    } else {
        Logger::error("Render context either does not exist or is not OpenGL");
    }
}

Vector2 WindowWin32::getSize() {
    RECT rect;
    GetClientRect(hwnd, &rect);
    return Vector2(float(rect.right - rect.left), float(rect.bottom - rect.top));
}

Vector2 WindowWin32::getPosition() {
    RECT rect;
    GetClientRect(hwnd, &rect);
    return Vector2(float(rect.left), float(rect.top));
}

bool WindowWin32::isKeyDown(KeyCode key) {
    // Fetch key state
    SHORT keyState = GetAsyncKeyState(keyCodeToVk(key));

    // Test high bit
    if (( 1 << 15 ) & keyState) return true; // if set, key was down when GetAsyncKeyState was called
    return false;
}

void WindowWin32::setMousePosition(const Vector2& position) {
    mouseGoal = position;
    reachingMouseGoal = true;
    
    RECT rect;
    GetClientRect(hwnd, &rect);
    POINT clientPoint = {static_cast<int>(position.x), static_cast<int>(rect.bottom - position.y)};
    ClientToScreen(hwnd, &clientPoint);

    SetCursorPos(clientPoint.x, clientPoint.y);

    lastMousePos = position;
    hasLastMousePos = true;
}

void WindowWin32::freezeMouse() {
    POINT mouse;
    GetCursorPos(&mouse);
    RECT rect;
    rect.left = mouse.x;
    rect.top = mouse.y;
    rect.right = mouse.x + 1;
    rect.bottom = mouse.y + 1;
    ClipCursor(&rect);
}

void WindowWin32::unfreezeMouse() {
    ClipCursor(NULL);
}

void WindowWin32::setCursor(Cursor cursor) {
    LPCSTR id = IDC_ARROW;
    switch (cursor) {
        case Cursor::HorizontalResize:
            id = IDC_SIZEWE;
            break;
        case Cursor::VerticalResize:
            id = IDC_SIZENS;
            break;
    }
    cursorId = id;

    // Update NOW
    SetCursor(LoadCursor(nullptr, cursorId));

    // And stay that way
    SetClassLongPtr(hwnd, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(LoadCursor(nullptr, cursorId)));
}

void WindowWin32::hideCursor() {
    ShowCursor(FALSE);
}

void WindowWin32::showCursor() {
    ShowCursor(TRUE);
}

void WindowWin32::setCapture() {
    SetCapture(hwnd);
}

void WindowWin32::releaseCapture() {
    ReleaseCapture();
}

void WindowWin32::pushInput(WindowInput input) {
    inputs.push_back(input);
}

WindowWin32::~WindowWin32() {
    if (glContext) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(glContext);
        glContext = nullptr;
    }
    if (hdc && hwnd) {
        ReleaseDC(hwnd, hdc);
        hdc = nullptr;
    }
    hwnd = nullptr;
}