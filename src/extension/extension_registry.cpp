#include "extension_registry.hpp"
#include <core/app.hpp>

MenuAction* ExtensionRegistry::registerMenuAction(MenuActionDefinition definition) {
    std::unique_ptr<MenuAction> action = std::make_unique<MenuAction>(definition);
    MenuAction* ptr = action.get();
    menuActions.push_back(std::move(action));

    onMenuActionRegistered.emit(ptr);

    return ptr;
}
ContextAction* ExtensionRegistry::registerContextAction(ContextActionDefinition definition) {
    std::unique_ptr<ContextAction> action = std::make_unique<ContextAction>(definition);
    ContextAction* ptr = action.get();
    contextActions.push_back(std::move(action));

    onContextActionRegistered.emit(ptr);

    return ptr;
}

Tool* ExtensionRegistry::registerTool(ToolDefinition definition) {
    std::unique_ptr<Tool> tool = std::make_unique<Tool>(definition);
    Tool* ptr = tool.get();
    tools.push_back(std::move(tool));

    if (!app.editor->getSelectedTool(definition.mode)) app.editor->setSelectedTool(definition.mode, ptr); // Select tool if none are selected

    onToolRegistered.emit(ptr);

    return ptr;
}