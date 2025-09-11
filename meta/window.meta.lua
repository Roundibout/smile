---@meta _

---@class Window
---@field renderer Renderer
Window = {}

---Connects a function to this window's update loop.
---@param callback fun(deltaTime: number)
function Window:ConnectUpdate(callback) end

---Connects a function to this window's render loop.
---@param callback fun()
function Window:ConnectRender(callback) end

---Connects a function to this window's WindowInput signal.
---@param callback fun(input: WindowInput)
function Window:ConnectInput(callback) end