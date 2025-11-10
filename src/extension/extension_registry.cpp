#include "extension_registry.hpp"
#include <core/app.hpp>

MenuAction* ExtensionRegistry::registerMenuAction(MenuActionConfig config) {
    std::unique_ptr<MenuAction> action = std::make_unique<MenuAction>(config);
    MenuAction* ptr = action.get();
    menuActions.push_back(std::move(action));
    onMenuActionRegistered.emit(ptr);
    return ptr;
}
ContextAction* ExtensionRegistry::registerContextAction(ContextActionConfig config) {
    std::unique_ptr<ContextAction> action = std::make_unique<ContextAction>(config);
    ContextAction* ptr = action.get();
    contextActions.push_back(std::move(action));
    onContextActionRegistered.emit(ptr);
    return ptr;
}

Tool* ExtensionRegistry::registerTool(ToolConfig config) {
    std::unique_ptr<Tool> tool = std::make_unique<Tool>(config);
    Tool* ptr = tool.get();
    tools.push_back(std::move(tool));
    onToolRegistered.emit(ptr);
    return ptr;
}