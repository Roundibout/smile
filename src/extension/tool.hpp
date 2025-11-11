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
    Select,
    Deselect,
    LeftMouseDown,
    LeftMouseUp,
    RightMouseDown,
    RightMouseUp,

    Count
};

struct ToolEvent {
    
};

struct ToolDefinition {
    std::string name = "Tool";
    Editor::Mode mode = Editor::Mode::Object;
    ToolCategory category = ToolCategory::Custom;
    
    std::array<std::vector<sol::function>, (size_t)ToolEventType::Count> eventHandlers;

    void connect(ToolEventType type, sol::protected_function handler) {
        eventHandlers[(size_t)type].push_back(std::move(handler));
    }
};

class Tool {
private:
    std::string name;
    Editor::Mode mode;
    ToolCategory category;

    std::array<std::vector<sol::protected_function>, (size_t)ToolEventType::Count> eventHandlers;
public:
    Tool(ToolDefinition definition);

    template<typename... Args>
    void fireEvent(ToolEventType type, Args&&... args) {
        for (auto& func : eventHandlers[(size_t)type]) {
            sol::protected_function_result result = func(std::forward<Args>(args)...);
            if (!result.valid()) {
                sol::error err = result;
                std::cerr << "Lua event error: " << err.what() << "\n";
            }
        }
    }

    std::string getName() {return name;}
    Editor::Mode getMode() {return mode;}
    ToolCategory getCategory() {return category;}
};