#pragma once

#include <string>
#include <array>
#include <vector>

#include <sol/sol.hpp>

#include <core/editor.hpp>

enum class ToolCategory {
    Select,
    Transform,
    Add,
    Modify,
    Custom
};

enum class ToolEventType {
    LeftMouseDown,
    LeftMouseUp,
    RightMouseDown,
    RightMouseUp,

    Count
};

struct ToolConfig {
    std::string name = "Tool";
    Editor::Mode mode = Editor::Mode::Object;
    ToolCategory category = ToolCategory::Custom;
};

struct ToolEvent {
    
};

class Tool {
private:
    ToolConfig config;

    std::array<std::vector<sol::function>, (size_t)ToolEventType::Count> eventHandlers;
public:
    Tool(ToolConfig config);

    void fireEvent(ToolEventType type, int x, int y) {
        for (auto& func : eventHandlers[(size_t)type]) {
            sol::protected_function_result result = func(x, y);
            if (!result.valid()) {
                sol::error err = result;
                std::cerr << "Lua event error: " << err.what() << "\n";
            }
        }
    }

    ToolConfig getConfig() {return config;}

    // Exposed to extension
    void connect(ToolEventType type, sol::function handler) {
        eventHandlers[(size_t)type].push_back(std::move(handler));
    }
};