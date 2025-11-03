---@meta _

---@class Extension
Extension = {}

---Registers a menu action.
---@param config MenuActionConfig
---@return MenuAction
function Extension:registerMenuAction(config) end

---Registers a context action.
---@param config ContextActionConfig
---@return ContextAction
function Extension:registerContextAction(config) end

---Registers a tool.
---@param config ToolConfig
---@return Tool
function Extension:registerTool(config) end