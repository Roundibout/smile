#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include <core/signal.hpp>

#include <extension/menu_action.hpp>
#include <extension/context_action.hpp>
#include <extension/tool.hpp>

class App;

class ExtensionRegistry {
public:
    using ListenerId = uint32_t;
private:
    App& app;

    std::vector<std::unique_ptr<MenuAction>> menuActions;

    std::vector<std::unique_ptr<ContextAction>> contextActions;

    std::vector<std::unique_ptr<Tool>> tools;
public:
    Signal<MenuAction*> onMenuActionRegistered;
    Signal<MenuAction*> onMenuActionRemoved;
    Signal<ContextAction*> onContextActionRegistered;
    Signal<ContextAction*> onContextActionRemoved;
    Signal<Tool*> onToolRegistered;
    Signal<Tool*> onToolRemoved;

    explicit ExtensionRegistry(App& app) : app(app) {}

    MenuAction* registerMenuAction(MenuActionConfig config);
    ContextAction* registerContextAction(ContextActionConfig config);
    Tool* registerTool(ToolConfig config);

    const std::vector<std::unique_ptr<MenuAction>>& getMenuActions() const {return menuActions;}
    const std::vector<std::unique_ptr<ContextAction>>& getContextActions() const {return contextActions;}
    const std::vector<std::unique_ptr<Tool>>& getTools() const {return tools;}
};