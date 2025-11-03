#pragma once

#include <string>

#include <sol/sol.hpp>

enum class ToolMode {
    Object,
    Edit
};

enum class ToolCategory {
    Select,
    Transform,
    Add,
    Modify,
    Custom
};

struct ToolConfig {
    std::string name = "Tool";
    ToolMode mode = ToolMode::Object;
    ToolCategory category = ToolCategory::Custom;
};

class Tool {
private:
    ToolConfig config;
public:
    Tool(ToolConfig config);
};