local toolDef = ToolDefinition.new()
toolDef.name = "Test"
toolDef.mode = EditorMode.Object
toolDef.category = ToolCategory.Select

toolDef:connect(ToolEvent.Select, function()
    print("Selected 💀");
end)

Extension:registerTool(toolDef)
Extension:registerTool(toolDef)

local toolDef2 = ToolDefinition.new()
toolDef2.name = "Test"
toolDef2.mode = EditorMode.Edit
toolDef2.category = ToolCategory.Select

Extension:registerTool(toolDef2)

print("Ok");