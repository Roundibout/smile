local toolConfig = ToolConfig.new()
toolConfig.name = "Test"
toolConfig.mode = EditorMode.Object
toolConfig.category = ToolCategory.Select

local tool1 = Extension:registerTool(toolConfig)
local tool2 = Extension:registerTool(toolConfig)

local toolConfig2 = ToolConfig.new()
toolConfig2.name = "Test"
toolConfig2.mode = EditorMode.Edit
toolConfig2.category = ToolCategory.Select

local tool3 = Extension:registerTool(toolConfig2)

print("Ok");

--[[
tool:connect(ToolEvent.LeftMouseDown, function(position)
    
end)
]]--