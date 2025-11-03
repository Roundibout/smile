---@meta _

---@class WindowConfig
---@field title string
---@field size Vector2
---@field color Color4
---@field resizable boolean
---@field maxSize Vector2?
---@field minSize Vector2
---@field maximized boolean
---@field minimized boolean
---@field maximizable boolean
---@field minimizable boolean
WindowConfig = {}

---Creates a new WindowConfig, used in creating windows.
---@return WindowConfig config
function WindowConfig.new() end

---@class Window
---@field renderer Renderer
Window = {}

---Connects a function to this window's update loop.
---@param callback fun(deltaTime: number, bounds: UIBounds)
function Window:connectUpdate(callback) end

---Connects a function to this window's render loop.
---@param callback fun(bounds: UIBounds)
function Window:connectRender(callback) end

---Connects a function to this window's WindowInput signal.
---@param callback fun(input: WindowInput, bounds: UIBounds)
function Window:connectInput(callback) end