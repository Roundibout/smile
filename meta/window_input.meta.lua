---@meta _

---@enum WindowInputType
WindowInputType = {
    MouseButtonDown = nil,
    MouseButtonUp = nil,
    MouseMove = nil,
    MouseScroll = nil,
    KeyDown = nil,
    KeyUp = nil,
    WindowResized = nil,
    WindowClosed = nil
}

---@enum MouseButton
MouseButton = {
    Left = nil,
    Right = nil,
    Middle = nil
}

---@class MouseInput
---@field button MouseButton?
---@field position Vector2
MouseInput = {}

---@enum KeyCode
KeyCode = {
    Q = nil, W = nil, E = nil, R = nil, T = nil, Y = nil, U = nil, I = nil, O = nil, P = nil,
    A = nil, S = nil, D = nil, F = nil, G = nil, H = nil, J = nil, K = nil, L = nil,
    Z = nil, X = nil, C = nil, V = nil, B = nil, N = nil, M = nil,

    One = nil, Two = nil, Three = nil, Four = nil, Five = nil, Six = nil, Seven = nil, Eight = nil, Nine = nil, Zero = nil,

    Space = nil,

    BackQuote = nil, Minus = nil, Equals = nil, LeftSquareBracket = nil, RightSquareBracket = nil, BackSlash = nil, Semicolon = nil, SingleQuote = nil, Comma = nil, Period = nil, Slash = nil,

    Escape = nil, F1 = nil, F2 = nil, F3 = nil, F4 = nil, F5 = nil, F6 = nil, F7 = nil, F8 = nil, F9 = nil, F10 = nil, F11 = nil, F12 = nil, Delete = nil,

    Tab = nil, CapsLock = nil, Shift = nil, Control = nil, Super = nil, Alt = nil, Enter = nil, Back = nil,

    Left = nil, Right = nil, Up = nil, Down = nil,

    Home = nil, End = nil, PageUp = nil, PageDown = nil,
    Unknown = nil
}

---@class WindowInput
---@field type WindowInputType
---@field mouse MouseInput?
---@field key KeyCode?
WindowInput = {}