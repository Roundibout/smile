---@meta _

---@class Renderer
Renderer = {}

---Draws a rectangle at the specified position (relative to the top-left corner) with the specified size and color.
---@param position Vector2
---@param size Vector2
---@param color Color4
function Renderer:DrawRect(position, size, color) end

---Flags the renderer for a render this frame.
function Renderer:Dirty() end

---Returns true if the renderer needs to redraw (or the renderer is "dirty").
---@return number dirty
function Renderer:IsDirty() end