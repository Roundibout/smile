---@meta _

---@enum ToolCategory
ToolCategory = {
    Select = 0,
    Transform = 1,
    Add = 2,
    Modify = 3,
    Custom = 4,
}

---@enum ToolEvent
ToolEvent = {
    LeftMouseDown = 0,
    LeftMouseUp = 1,
    RightMouseDown = 2,
    RightMouseUp = 3,
}

---@class ToolConfig
---@field name string
---@field mode EditorMode
---@field category ToolCategory
ToolConfig = {}

---Creates a new ToolConfig filled with default fields.
---@return ToolConfig
function ToolConfig.new() end

---@class Tool
Tool = {}

---Connects a function to a tool event.
---@param event ToolEvent
---@param handler function
function Tool:connect(event, handler) end