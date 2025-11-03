local toolConfig = ToolConfig.new()
toolConfig.name = "Test"
toolConfig.mode = ToolMode.Object
toolConfig.category = ToolCategory.Add

local tool = Extension:registerTool(toolConfig)

print("Done");