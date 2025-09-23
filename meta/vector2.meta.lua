---@meta _

---@class Vector2
---@field x number
---@field y number
---@operator add(Vector2): Vector2
---@operator sub(Vector2): Vector2
---@operator mul(Vector2): Vector2
---@operator div(Vector2): Vector2
---@operator idiv(Vector2): Vector2
----@operator eq(Vector2): boolean
---@operator unm(): Vector2
Vector2 = {}

---Creates a blank Vector2.
---@return Vector2
function Vector2.new() end

---Creates a new Vector2 consisting of two numbers.
---@param x number The first dimension.
---@param y number The second dimension.
---@return Vector2
function Vector2.new(x, y) end