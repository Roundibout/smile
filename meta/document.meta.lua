---@meta _

---@class Canvas
Canvas = {}

---@class Point
---@field id integer
---@field x number
---@field y number
Point = {}

---@class Line
---@field id integer
---@field point1 integer
---@field point2 integer
Line = {}

---@class Instance
---@field name string
Instance = {}

---Gets the name of the instance.
---@return string
function Instance:getName() end

---@class Object : Instance
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

---Gets an array of all points within this object.
---@return Point[]
function Object:getPoints() end

---@class Document
Document = {}

---Creates a canvas.
---@param rect Rect
---@return Canvas
function Document:createCanvas(rect) end

---Creates an object.
---@return Object
function Document:createObject() end

---Gets an array of all instances within this document.
---@return Instance[]
function Document:getContents() end

---@class documents
---@field current Document
documents = {}