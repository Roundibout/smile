---@meta _

---@class Renderer
Renderer = {}

---Draws a rectangle at the specified position (relative to the bottom-left corner) with the specified size and color.
---@param position Vector2
---@param size Vector2
---@param color Color4
function Renderer:DrawRect(position, size, color) end

---Draws a rounded rectangle at the specified position (relative to the bottom-left corner) with the specified size, color, and corner radius.
---@param position Vector2
---@param size Vector2
---@param color Color4
---@param corner UIDim
function Renderer:DrawRoundedRect(position, size, color, corner) end

---Draws text at the specified position (relative to the bottom-left corner) with the specified font (file path), size, (height) and color.
---@param text string
---@param position Vector2
---@param font string
---@param size integer
---@param color Color4
function Renderer:DrawText(text, position, font, size, color) end

---Flags the renderer for a render this frame.
function Renderer:Dirty() end

---Returns true if the renderer needs to redraw (or the renderer is "dirty").
---@return number dirty
function Renderer:IsDirty() end