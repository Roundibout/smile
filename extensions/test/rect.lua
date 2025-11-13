local def = ToolDefinition.new()
def.name = "Rectangle"
def.mode = EditorMode.Object
def.category = ToolCategory.Add

local firstPos = Vector2.new()

---@param position Vector2
def:connect(ToolEvent.LeftMouseDown, function(position)
    firstPos = position

    print(firstPos.x, firstPos.y)
    print("Down")
end)

---@param position Vector2
def:connect(ToolEvent.LeftMouseUp, function(position)
    local doc = documents.current
    local object = doc:createObject()

    print(firstPos.x, firstPos.y)

    local point1 = object:createPoint(Vector2.new(firstPos.x, firstPos.y))
    local point2 = object:createPoint(Vector2.new(position.x, firstPos.y))
    local point3 = object:createPoint(Vector2.new(position.x, position.y))
    local point4 = object:createPoint(Vector2.new(firstPos.x, position.y))

    local line1 = object:createLine(point1, point2)
    local line2 = object:createLine(point2, point3)
    local line3 = object:createLine(point3, point4)
    local line4 = object:createLine(point4, point1)
    print("Up")
end)

extension:registerTool(def)