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

void pushInput(HWND hwnd, WindowInput input) {
    WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Get the window

    // Push the input to the queue
    window->pushInput(input);
}

KeyCode vkToKeyCode(WPARAM vk) {
    switch (vk) { // Crap
        case 'Q': return KeyCode::Q;
        case 'W': return KeyCode::W;
        case 'E': return KeyCode::E;
        case 'R': return KeyCode::R;
        case 'T': return KeyCode::T;
        case 'Y': return KeyCode::Y;
        case 'U': return KeyCode::U;
        case 'I': return KeyCode::I;
        case 'O': return KeyCode::O;
        case 'P': return KeyCode::P;
        case 'A': return KeyCode::A;
        case 'S': return KeyCode::S;
        case 'D': return KeyCode::D;
        case 'F': return KeyCode::F;
        case 'G': return KeyCode::G;
        case 'H': return KeyCode::H;
        case 'J': return KeyCode::J;
        case 'K': return KeyCode::K;
        case 'L': return KeyCode::L;
        case 'Z': return KeyCode::Z;
        case 'X': return KeyCode::X;
        case 'C': return KeyCode::C;
        case 'V': return KeyCode::V;
        case 'B': return KeyCode::B;
        case 'N': return KeyCode::N;
        case 'M': return KeyCode::M;

        case '1': return KeyCode::One;
        case '2': return KeyCode::Two;
        case '3': return KeyCode::Three;
        case '4': return KeyCode::Four;
        case '5': return KeyCode::Five;
        case '6': return KeyCode::Six;
        case '7': return KeyCode::Seven;
        case '8': return KeyCode::Eight;
        case '9': return KeyCode::Nine;
        case '0': return KeyCode::Zero;

        case VK_SPACE: return KeyCode::Space;

        case VK_OEM_3: return KeyCode::BackQuote;
        case VK_OEM_MINUS: return KeyCode::Minus;
        case VK_OEM_PLUS: return KeyCode::Equals;
        case VK_OEM_4: return KeyCode::LeftSquareBracket;
        case VK_OEM_6: return KeyCode::RightSquareBracket;
        case VK_OEM_5: return KeyCode::BackSlash;
        case VK_OEM_1: return KeyCode::Semicolon;
        case VK_OEM_7: return KeyCode::SingleQuote;
        case VK_OEM_COMMA: return KeyCode::Comma;
        case VK_OEM_PERIOD: return KeyCode::Period;
        case VK_OEM_2: return KeyCode::Slash;

        case VK_ESCAPE: return KeyCode::Escape;
        case VK_F1: return KeyCode::F1;
        case VK_F2: return KeyCode::F2;
        case VK_F3: return KeyCode::F3;
        case VK_F4: return KeyCode::F4;
        case VK_F5: return KeyCode::F5;
        case VK_F6: return KeyCode::F6;
        case VK_F7: return KeyCode::F7;
        case VK_F8: return KeyCode::F8;
        case VK_F9: return KeyCode::F9;
        case VK_F10: return KeyCode::F10;
        case VK_F11: return KeyCode::F11;
        case VK_F12: return KeyCode::F12;
        case VK_DELETE: return KeyCode::Delete;

        case VK_TAB: return KeyCode::Tab;
        case VK_CAPITAL: return KeyCode::CapsLock;
        case VK_SHIFT: return KeyCode::Shift;
        case VK_LSHIFT: return KeyCode::Shift;
        case VK_RSHIFT: return KeyCode::Shift;
        case VK_CONTROL: return KeyCode::Control;
        case VK_LCONTROL: return KeyCode::Control;
        case VK_RCONTROL: return KeyCode::Control;
        case VK_LWIN: return KeyCode::Super;
        case VK_RWIN: return KeyCode::Super;
        case VK_MENU: return KeyCode::Alt;
        case VK_LMENU: return KeyCode::Alt;
        case VK_RMENU: return KeyCode::Alt;
        case VK_RETURN: return KeyCode::Enter;
        case VK_BACK: return KeyCode::Back;

        case VK_LEFT: return KeyCode::Left;
        case VK_RIGHT: return KeyCode::Right;
        case VK_UP: return KeyCode::Up;
        case VK_DOWN: return KeyCode::Down;

        case VK_HOME: return KeyCode::Home;
        case VK_END: return KeyCode::End;
        case VK_PRIOR: return KeyCode::PageUp;
        case VK_NEXT: return KeyCode::PageDown;
    }
    return KeyCode::Unknown;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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
            App::get().destroyWindow(window->getId());
            PostQuitMessage(0);
            return 0;
        }

        // FIX FOR STALLING DURING RESIZE AND MOVE

        case WM_SIZE:
            App::get().step();
            return 0;
        case WM_MOVE:
            App::get().step();
            return 0;
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

            WindowInput input;
            input.type = WindowInputType::MouseMove;
            input.mouse.position = Vector2(GET_X_LPARAM(lParam), int(window->getSize().y) - GET_Y_LPARAM(lParam));

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
            
            POINT point;
            point.x = GET_X_LPARAM(lParam);
            point.y = GET_Y_LPARAM(lParam);
            ScreenToClient(hwnd, &point); // convert to window-relative space because its in display-relative space for some reason
            input.mouse.position = Vector2(float(point.x), window->getSize().y - float(GET_Y_LPARAM(lParam))); // convert from weird Windows types to real values

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
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = wc.hIcon;

    RegisterClassEx(&wc);
    classRegistered = true;
}

// Win32 window constructor
WindowWin32::WindowWin32(const uint32_t& i, const WindowConfig& c) : WindowImpl(i, c) {
    SetProcessDPIAware(); // Set this so it isn't blurry

    // Get the handle to Smile
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    // Register a window class with Windows if one has not been registered yet
    registerWindowClass(hInstance);

    // Create window style from config
    DWORD style = WS_OVERLAPPEDWINDOW;

    if (!config.resizable)
        style &= ~WS_SIZEBOX;

    if (!config.maximizable)
        style &= ~WS_MAXIMIZEBOX;

    if (!config.minimizable)
        style &= ~WS_MINIMIZEBOX;
    
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

std::queue<WindowInput> WindowWin32::update() {
    MSG msg;
    // Process all pending messages
    while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Copy inputs to a new queue for use outside
    std::queue<WindowInput> inputsCopy;
    std::swap(inputsCopy, inputs);

    return inputsCopy;
}

void WindowWin32::bindGLContext() {
    if (rbackendSet) {
        std::cout << "Render context already bound" << std::endl;
        return;
    }

    // Set variables for the backend used and that it is set
    rbackend = RenderBackend::GL;
    rbackendSet = true;

    // Create a PIXEL FORMAT DESCRIPTOR
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pf = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pf, &pfd);

    // Create dummy context (we need it to move out of caveman GL)
    HGLRC dummyCtx = wglCreateContext(hdc);
    wglMakeCurrent(hdc, dummyCtx); // Make it current so we can use it

    // Load WGL extensions (needed for core context creation)
    if (!gladLoaderLoadWGL(hdc)) {
        std::cerr << "Failed to load WGL extensions\n";
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(dummyCtx);
        return;
    }

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
        std::cerr << "Failed to load OpenGL" << std::endl;
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(glContext);
    }
}

void WindowWin32::makeGLCurrent() {
    if (rbackendSet && rbackend == RenderBackend::GL) {
        // Just make the context current
        wglMakeCurrent(hdc, glContext);
    } else {
        std::cerr << "Render context either does not exist or is not OpenGL" << std::endl;
    }
}

void WindowWin32::swapGLBuffers() {
    if (rbackendSet && rbackend == RenderBackend::GL) {
        // Just swap buffers
        SwapBuffers(hdc);
    } else {
        std::cerr << "Render context either does not exist or is not OpenGL" << std::endl;
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

void WindowWin32::pushInput(WindowInput input) {
    inputs.push(input);
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