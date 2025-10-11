---@meta _

---@enum WindowInputType
WindowInputType = {
    MouseButtonDown = 0,
    MouseButtonUp = 1,
    MouseMove = 2,
    MouseScroll = 3,
    KeyDown = 4,
    KeyUp = 5,
    WindowResized = 6,
    WindowMoved = 7,
    WindowClosed = 8
}

---@enum MouseButton
MouseButton = {
    Left = 0,
    Right = 1,
    Middle = 2
}

---@class MouseInput
---@field button MouseButton?
---@field position Vector2
---@field scroll Vector2?
MouseInput = {}

---@enum KeyCode
KeyCode = {
    Q = 0, W = 1, E = 2, R = 3, T = 4, Y = 5, U = 6, I = 7, O = 8, P = 9,
    A = 10, S = 11, D = 12, F = 13, G = 14, H = 15, J = 16, K = 17, L = 18,
    Z = 19, X = 20, C = 21, V = 22, B = 23, N = 24, M = 25,

    One = 26, Two = 27, Three = 28, Four = 29, Five = 30, Six = 31, Seven = 32, Eight = 33, Nine = 34, Zero = 35,

    Space = 36,

    BackQuote = 37, Minus = 38, Equals = 39, LeftSquareBracket = 40, RightSquareBracket = 41,
    BackSlash = 42, Semicolon = 43, SingleQuote = 44, Comma = 45, Period = 46, Slash = 47,

    Escape = 48, F1 = 49, F2 = 50, F3 = 51, F4 = 52, F5 = 53, F6 = 54, F7 = 55, F8 = 56,
    F9 = 57, F10 = 58, F11 = 59, F12 = 60, Delete = 61,

    Tab = 62, CapsLock = 63, Shift = 64, Control = 65, Super = 66, Alt = 67, Enter = 68, Back = 69,

    Left = 70, Right = 71, Up = 72, Down = 73,

    Home = 74, End = 75, PageUp = 76, PageDown = 77,

    Unknown = 78
}

---@class WindowChangeInput
---@field position Vector2?
---@field size Vector2?
WindowChangeInput = {}

---@class WindowInput
---@field type WindowInputType
---@field mouse MouseInput?
---@field key KeyCode?
---@field window WindowChangeInput?
WindowInput = {}