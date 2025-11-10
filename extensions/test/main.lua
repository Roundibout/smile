local toolConfig = ToolConfig.new()
toolConfig.name = "Test"
toolConfig.mode = EditorMode.Object
toolConfig.category = ToolCategory.Select

local tool1 = Extension:registerTool(toolConfig)
local tool2 = Extension:registerTool(toolConfig)
local tool3 = Extension:registerTool(toolConfig)

print("Ok");

--[[
tool:connect(ToolEvent.LeftMouseDown, function(position)
    
end)
]]--