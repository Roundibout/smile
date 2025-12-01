#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>

#include <sol/sol.hpp>

#include "type/vector2.hpp"
#include "type/rect.hpp"
#include "type/color4.hpp"
#include "type/ui_types.hpp"
#include "type/point.hpp"
#include "type/line.hpp"
#include "type/shape.hpp"

#include "core/editor.hpp"
#include "document/document_manager.hpp"
#include "document/document.hpp"
#include "document/canvas.hpp"
#include "document/object.hpp"
#include "window/window.hpp"
#include "window/window_input.hpp"
#include "ui/theme.hpp"
#include "extension/extension_registry.hpp"
#include "extension/menu_action.hpp"
#include "extension/context_action.hpp"
#include "extension/tool.hpp"

class App;

using Id = uint32_t;

class Extension {
private:
    App& app;

    Id id;
    std::string name;
    std::string folder;

    sol::state lua;

    std::vector<MenuAction*> menuActions;
    std::vector<ContextAction*> contextActions;
    std::vector<Tool*> tools;
public:
    Extension(App& app, Id id, std::string name, std::string path);

    bool load();
    bool unload();

    // Register functions
    void registerMenuAction(MenuActionDefinition definition);
    void registerContextAction(ContextActionDefinition definition);
    void registerTool(ToolDefinition definition);
};