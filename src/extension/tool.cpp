#include "tool.hpp"

Tool::Tool(ToolDefinition definition)
    : name(std::move(definition.name)),
      mode(definition.mode),
      category(definition.category)
{
    // Copy over event handlers
    for (size_t i = 0; i < (size_t)ToolEventType::Count; ++i) {
        for (auto& func : definition.eventHandlers[i]) {
            // Wrap each one in a protected_function
            eventHandlers[i].emplace_back(sol::protected_function(func));
        }
    }
}