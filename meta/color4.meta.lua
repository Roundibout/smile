---@meta _

---@class Color4
---@field r number
---@field g number
---@field b number
---@field a number
Color4 = {}

---Creates a new Color4 consisting of four numbers for red, green, blue, and alpha (opacity).
---@param r number The red channel.
---@param g number The green channel.
---@param b number The blue channel.
---@param a number The alpha (opacity) channel.
---@return Color4
---If no arguments are provided, a white, opaque Color4 will be created.
---@overload fun(): Color4
function Color4.new(r, g, b, a) end