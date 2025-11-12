---@meta _

---@class Canvas
Canvas = {}

---@class Object
Object = {}

---Creates a point at the specified position.
---@param position Vector2
---@return integer
function Object:createPoint(position) end

---Creates a line between the specified points.
---@param point1 integer
---@param point2 integer
---@return integer
function Object:createLine(point1, point2) end

---@class Document
Document = {}

---Creates a canvas.
---@param rect Rect
---@return Canvas
function Document:createCanvas(rect) end

---Creates an object.
---@return Object
function Document:createObject() end

---@class documents
---@field current Document
documents = {}