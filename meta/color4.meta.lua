---@meta _

---@class Color4
---@field r number
---@field g number
---@field b number
---@field a number
Color4 = {}

---Creates a new white and opaque Color4.
function Color4.new() end

---Creates a new opaque Color4 out of three numbers for red, green, and blue.
---@param r number The red channel.
---@param g number The green channel.
---@param b number The blue channel.
---@return Color4
function Color4.new(r, g, b) end

---Creates a new Color4 out of four numbers for red, green, blue, and alpha (opacity).
---@param r number The red channel.
---@param g number The green channel.
---@param b number The blue channel.
---@param a number The alpha (opacity) channel.
---@return Color4
function Color4.new(r, g, b, a) end