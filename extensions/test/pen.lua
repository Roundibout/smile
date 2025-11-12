local def = ToolDefinition.new()
def.name = "Pen"
def.mode = EditorMode.Object
def.category = ToolCategory.Select

---@param position Vector2
def:connect(ToolEvent.LeftMouseDown, function(position)
    print(position.x, position.y)
end)

extension:registerTool(def)