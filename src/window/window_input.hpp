#pragma once

#include <type/vector2.hpp>

// This exists so we don't need to write all of these multiple times in different places
#define KEY_CODE_LIST \
    /* Letters */ \
    X(Q) X(W) X(E) X(R) X(T) X(Y) X(U) X(I) X(O) X(P) \
    X(A) X(S) X(D) X(F) X(G) X(H) X(J) X(K) X(L) \
    X(Z) X(X) X(C) X(V) X(B) X(N) X(M) \
    /* Numbers */ \
    X(One) X(Two) X(Three) X(Four) X(Five) X(Six) X(Seven) X(Eight) X(Nine) X(Zero) \
    /* Space */ \
    X(Space) \
    /* Symbols */ \
    X(BackQuote) X(Minus) X(Equals) X(LeftSquareBracket) X(RightSquareBracket) X(BackSlash) \
    X(Semicolon) X(SingleQuote) X(Comma) X(Period) X(Slash) \
    /* Top row */ \
    X(Escape) X(F1) X(F2) X(F3) X(F4) X(F5) X(F6) X(F7) X(F8) X(F9) X(F10) X(F11) X(F12) X(Delete) \
    /* Commands & modifiers */ \
    X(Tab) X(CapsLock) X(Shift) X(Control) X(Super) X(Alt) X(Enter) X(Back) \
    /* Arrows */ \
    X(Left) X(Right) X(Up) X(Down) \
    /* Other */ \
    X(Home) X(End) X(PageUp) X(PageDown) \
    X(Unknown)


enum class WindowInputType {
    MouseButtonDown,
    MouseButtonUp,
    MouseMove,
    MouseScroll,
    KeyDown,
    KeyUp,
    WindowResized,
    WindowMoved,
    WindowClosed
};

enum class MouseButton {
    Left,
    Right,
    Middle
};

struct MouseInput {
    Vector2 position;
    union {
        MouseButton button;
        Vector2 delta;
        Vector2 scroll;
    };

    MouseInput() {}
};

enum class KeyCode {
#define X(name) name,
    KEY_CODE_LIST
#undef X
};

struct WindowChangeInput {
    union {
        Vector2 position;
        Vector2 size;
    };

    WindowChangeInput() {}
};

struct WindowInput {
    WindowInputType type;
    union {
        MouseInput mouse;
        KeyCode key;
        WindowChangeInput window;
    };

    WindowInput() {}
};