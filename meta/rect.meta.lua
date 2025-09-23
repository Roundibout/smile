---@meta _

---@class Rect
---@field position Vector2
---@field size Vector2
----@operator eq(Rect): boolean
Rect = {}

---Creates a new degenerate Rect at the origin.
---@return Rect
function Rect.new() end

---Creates a new Rect with a position and size.
---@param position Vector2
---@param size Vector2
---@return Rect
function Rect.new(position, size) end

---Creates a new Rect with each x and y component of its position and size.
---@param positionX number
---@param positionY number
---@param sizeX number
---@param sizeY number
---@return Rect
---@overload fun(): Rect
function Rect.new(positionX, positionY, sizeX, sizeY) end