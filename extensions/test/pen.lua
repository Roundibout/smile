local def = ToolDefinition.new()
def.name = "Pen"
def.mode = EditorMode.Object
def.category = ToolCategory.Modify

local object = nil
local lastPoint = nil
local firstPoint = nil

---@param position Vector2
def:connect(ToolEvent.LeftMouseDown, function(position)
    local doc = documents.current

    local closestObject = nil
    local closestPoint = nil
    local closestDistance = nil

    for i, object in pairs(doc:getContents()) do
        -- Assuming everything is an object
        ---@cast object Object
        print(object:getName())
        for i, point in pairs(object:getPoints()) do
            local dx, dy = position.x - point.x, position.y - point.y
            local distance = dx*dx + dy*dy
            if closestDistance == nil then
                closestObject = object
                closestPoint = point.id
                closestDistance = distance
            elseif distance < closestDistance then
                closestObject = object
                closestPoint = point.id
                closestDistance = distance
            end
        end
    end

    if object == nil then
        if closestPoint ~= nil and closestDistance <= 5 then
            object = closestObject
            lastPoint = closestPoint
        else
            object = doc:createObject()
            lastPoint = object:createPoint(position)
            firstPoint = lastPoint
        end
    else
        if closestObject == object and closestPoint and closestDistance <= 5 then
            object:createLine(lastPoint, closestPoint)
            if closestPoint == firstPoint then
                object = nil
                lastPoint = nil
                firstPoint = nil
            else
                lastPoint = closestPoint
            end
        else
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