local penDef = ToolDefinition.new()
penDef.name = "Pen"
penDef.mode = EditorMode.Object
penDef.category = ToolCategory.Select

---@param position Vector2
penDef:connect(ToolEvent.LeftMouseDown, function(position)
    
end)

Extension:registerTool(penDef)