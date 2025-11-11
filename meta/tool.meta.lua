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
    Select = 0,
    Deselect = 1,
    LeftMouseDown = 2,
    LeftMouseUp = 3,
    RightMouseDown = 4,
    RightMouseUp = 5,
}

---@class ToolDefinition
---@field name string
---@field mode EditorMode
---@field category ToolCategory
ToolDefinition = {}

---Creates a new ToolConfig filled with default fields.
---@return ToolDefinition
function ToolDefinition.new() end

---Connects a function to a tool definition.
---@param event ToolEvent
---@param handler function
function ToolDefinition:connect(event, handler) end

---@class Tool
Tool = {}