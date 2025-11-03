#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <filesystem>

#include <sol/sol.hpp>

#include <datatypes/vector2.hpp>
#include <datatypes/rect.hpp>
#include <datatypes/color4.hpp>
#include <datatypes/ui_types.hpp>

#include <window/window.hpp>
#include <window/window_input.hpp>
#include <ui/theme.hpp>
#include <app/app.hpp>

#include <extension/menu_action.hpp>
#include <extension/context_action.hpp>
#include <extension/tool.hpp>

using Id = uint32_t;

class Extension {
private:
    Id id;
    std::string name;
    std::string folder;

    sol::state lua;

    std::vector<std::shared_ptr<MenuAction>> menuActions;
    std::vector<std::shared_ptr<ContextAction>> contextActions;
    std::vector<std::shared_ptr<Tool>> tools;

    std::shared_ptr<MenuAction> registerMenuAction(MenuActionConfig config);
    std::shared_ptr<ContextAction> registerContextAction(ContextActionConfig config);
    std::shared_ptr<Tool> registerTool(ToolConfig config);
public:
    Extension(Id id, std::string name, std::string path);

    bool load();
    bool unload();
};