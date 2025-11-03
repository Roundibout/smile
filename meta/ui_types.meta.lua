---@meta _

---@class UIDim
---@field scale number
---@field offset integer
---@operator add(UIDim): UIDim
---@operator sub(UIDim): UIDim
----@operator eq(UIDim): boolean
UIDim = {}

---Creates a blank UIDim.
---@return UIDim
function UIDim.new() end

---Creates a UIDim from a scale and an offset.
---@param scale number
---@param offset integer
---@return UIDim
function UIDim.new(scale, offset) end

---@class UIDim2
---@field x UIDim
---@field y UIDim
---@operator add(UIDim2): UIDim2
---@operator sub(UIDim2): UIDim2
----@operator eq(UIDim2): boolean
UIDim2 = {}

---Creates a blank UIDim2.
---@return UIDim2
function UIDim2.new() end

---Creates a UIDim2 from two UIDims.
---@param x UIDim
---@param y UIDim
---@return UIDim2
function UIDim2.new(x, y) end

---Creates a UIDim2 from each scale and offset component of its UIDims.
---@param scaleX number
---@param offsetX number
---@param scaleY number
---@param offsetY number
---@return UIDim2
function UIDim2.new(scaleX, offsetX, scaleY, offsetY) end

---@class UIRect
---@field position UIDim2
---@field size UIDim2
----@operator eq(UIRect): boolean
UIRect = {}

---Creates a degenerate UIRect at the origin.
---@return UIRect
function UIRect.new() end

---Creates a UIRect from a position and size.
---@param position UIDim2
---@param size UIDim2
---@return UIRect
function UIRect.new(position, size) end

---@class AbsoluteLayout
---@field rect Rect
---@field cornerRT number
---@field cornerLT number
---@field cornerRB number
---@field cornerLB number
AbsoluteLayout = {}

---Creates a degenerate AbsoluteLayout at the origin.
---@return AbsoluteLayout
function AbsoluteLayout.new() end

---Creates an AbsoluteLayout from a Rect.
---@param rect Rect
---@return AbsoluteLayout
function AbsoluteLayout.new(rect) end

---@class UILayout
---@field rect UIRect
---@field cornerRT UIDim
---@field cornerLT UIDim
---@field cornerRB UIDim
---@field cornerLB UIDim
UILayout = {}

---Creates a degenerate UILayout at the origin.
---@return UILayout
function UILayout.new() end

---Creates a UILayout from a UIRect.
---@param rect UIRect
---@return UILayout
function UILayout.new(rect) end

---Sets all four corners of the layout to the same value.
---@param corner UIDim
function UILayout:setCorners(corner) end

---@class UIBounds
---@field absolute Vector2
---@field layout UILayout
UIBounds = {}

---Creates a blank UIBounds with zero absolute size and a degenerate UILayout at the origin.
---@return UIBounds
function UIBounds.new() end

---Creates a UIBounds from an absolute size with a degenerate UILayout at the origin.
---@param absolute Vector2
---@return UIBounds
function UIBounds.new(absolute) end

---Creates a UIBounds from an absolute size and a UILayout.
---@param absolute Vector2
---@param layout UILayout
---@return UIBounds
function UIBounds.new(absolute, layout) end

---@enum UIStrokeAlignment
UIStrokeAlignment = {
    Outside = 0,
    Middle = 1,
    Inside = 2,
}