---@meta _

---@enum ToolMode
ToolMode = {
    Object = 0,
    Edit = 1
};

---@enum ToolCategory
ToolCategory = {
    Select = 0,
    Transform = 1,
    Add = 2,
    Modify = 3,
    Custom = 4
};

---@class ToolConfig
---@field name string
---@field mode ToolMode
---@field category ToolCategory
ToolConfig = {}

---Creates a new ToolConfig filled with default fields.
---@return ToolConfig
function ToolConfig.new() end

---@class Tool
Tool = {}