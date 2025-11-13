local def = ToolDefinition.new()
def.name = "Pen"
def.mode = EditorMode.Object
def.category = ToolCategory.Modify

local object = nil
local firstPoint = nil
local firstPointPosition = Vector2.new()
local lastPoint = nil

---@param position Vector2
def:connect(ToolEvent.LeftMouseDown, function(position)
    local doc = documents.current
    if object == nil then
        object = doc:createObject()
        lastPoint = object:createPoint(position)
        firstPoint = lastPoint
        firstPointPosition = position
    else
        local dx, dy = position.x - firstPointPosition.x, position.y - firstPointPosition.y
        if dx*dx + dy*dy <= 4 then
            print("end")
            object:createLine(lastPoint, firstPoint)
            object = nil
            firstPoint = nil
            lastPoint = nil
        else
            print("continue")
            local point = object:createPoint(position)
            object:createLine(lastPoint, point)
            lastPoint = point
        end
    end
end)

def:connect(ToolEvent.Deselect, function()
    object = nil
    firstPoint = nil
    lastPoint = nil
end)

extension:registerTool(def)