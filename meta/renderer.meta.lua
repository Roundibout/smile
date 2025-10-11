---@meta _

---@class Renderer
Renderer = {}

---Applies UIBounds on a position, returning the position in absolute space.
---@param position UIDim2
---@param bounds UIBounds
---@return Vector2
function Renderer:ResolvePosition(position, bounds) end

---Applies UIBounds on a UILayout, returning the layout in absolute space as an AbsoluteLayout.
---@param layout UILayout
---@param bounds UIBounds
---@return AbsoluteLayout
function Renderer:ResolvePosition(layout, bounds) end

---Applies a UILayout on a UIBounds, returning a new UIBounds confined to the UILayout in UI space within the given UIBounds.
---@param bounds UIBounds
---@param layout UILayout
---@return UIBounds
function Renderer:ApplyLayout(bounds, layout) end

---Draws a triangle with the specified three points, bounds, and color.
---@param position1 UIDim2
---@param position2 UIDim2
---@param position3 UIDim2
---@param bounds UIBounds
---@param color Color4
function Renderer:DrawTriangle(position1, position2, position3, bounds, color) end

---Draws a quadrilateral with the specified four points, bounds, and color.
---@param position1 UIDim2
---@param position2 UIDim2
---@param position3 UIDim2
---@param position4 UIDim2
---@param bounds UIBounds
---@param color Color4
function Renderer:DrawQuad(position1, position2, position3, position4, bounds, color) end

---Draws a rectangle with the specified layout, bounds, and color.
---@param layout UILayout
---@param bounds UIBounds
---@param color Color4
function Renderer:DrawRect(layout, bounds, color) end

---Draws a stroke rectangle with the specified layout, bounds, color, stroke thickness, stroke color, and stroke alignment.
---@param layout UILayout
---@param bounds UIBounds
---@param color Color4
---@param stroke integer
---@param strokeColor Color4
---@param strokeAlignment UIStrokeAlignment
function Renderer:DrawStrokeRect(layout, bounds, color, stroke, strokeColor, strokeAlignment) end

---Draws a rounded rectangle with the specified layout, bounds, and color.
---@param layout UILayout
---@param bounds UIBounds
---@param color Color4
function Renderer:DrawRoundedRect(layout, bounds, color) end

---Draws a rounded stroke rectangle with the specified layout, bounds, color, stroke thickness, stroke color, and stroke alignment.
---@param layout UILayout
---@param bounds UIBounds
---@param color Color4
---@param stroke integer
---@param strokeColor Color4
---@param strokeAlignment UIStrokeAlignment
function Renderer:DrawRoundedRect(layout, bounds, color, stroke, strokeColor, strokeAlignment) end

---Draws text with the specified position, bounds, text, font (file path), size (height), and color.
---@param position UIDim2
---@param bounds UIBounds
---@param text string
---@param font string
---@param size integer
---@param color Color4
function Renderer:DrawText(position, bounds, text, font, size, color) end

---Flags the renderer for a render this frame.
function Renderer:Dirty() end

---Returns true if the renderer needs to redraw (or the renderer is "dirty").
---@return number dirty
function Renderer:IsDirty() end