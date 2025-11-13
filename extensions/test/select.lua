local def = ToolDefinition.new()
def.name = "Select"
def.mode = EditorMode.Object
def.category = ToolCategory.Select

---@param position Vector2
def:connect(ToolEvent.LeftMouseDown, function(position)
    local doc = documents.current

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
                closestPoint = point.id
                closestDistance = distance
            elseif distance < closestDistance then
                closestPoint = point.id
                closestDistance = distance
            end
        end
    end

    print("closest point:", tostring(closestPoint))
    print("distance:", tostring(math.sqrt(closestDistance)))
end)

extension:registerTool(def)