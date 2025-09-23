---@meta _

---@class UIDim
---@field scale number
---@field offset integer
----@operator eq(UIDim): boolean
UIDim = {}

---Creates a blank UIDim.
---@return UIDim
function UIDim.new() end

---Creates a UIDim consisting of a scale and an offset.
---@param scale number
---@param offset integer
---@return UIDim
function UIDim.new(scale, offset) end

---@class UIDim2
---@field x UIDim
---@field y UIDim
----@operator eq(UIDim2): boolean
UIDim2 = {}

---Creates a blank UIDim2.
---@return UIDim2
function UIDim2.new() end

---Creates a UIDim2 consisting of two UIDims.
---@param x UIDim
---@param y UIDim
---@return UIDim2
function UIDim2.new(x, y) end

---Creates a UIDim2 with each scale and offset component of its UIDims.
---@param scaleX number
---@param offsetX integer
---@param scaleY number
---@param offsetY integer
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

---Creates a UIRect with a position and size.
---@param position UIDim2
---@param size UIDim2
---@return UIRect
function UIRect.new(position, size) end