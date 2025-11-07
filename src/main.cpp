/*
File:
    main.cpp
Authors:
    Lucas
Purpose:
    The entry point of Smile
    Initializes Lua and starts the program
License:
    MIT (see LICENSE file)
*/

#include <memory>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#endif

#include <app/app.hpp>
#include <extension/extension_manager.hpp>
#include <ui/theme.hpp>
#include <util/logger.hpp>

#include <ui/widgets/panels/panel_container.hpp>
#include <ui/widgets/viewport/viewport.hpp>
#include <ui/widgets/console/console.hpp>
#include <ui/widgets/tab/tab_bar.hpp>

//extern void register_bindings(sol::state& lua);

int main(int argc, char *argv[]) {
    Logger::print("//// Initializing ////\n");

    // Windows stuff
    #ifdef _WIN32
        timeBeginPeriod(1); // set minimum timer resolution
    #endif

    Logger::print("//// Loading extensions ////\n");

    ExtensionManager::get().loadAll();

    Logger::print("\n//// Starting smile ////\n");

    WindowConfig config;
    config.title = "Smile";
    config.size = Vector2(1920, 1080);
    config.minSize = Vector2(400, 400);
    config.color = Theme::color(ThemeColor::WindowBackground);
    config.maximized = true;

    std::shared_ptr<Window> window = App::get().createWindow(config);

    PanelContainer* container = window->addWidget<PanelContainer>(UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 30), UIDim2(1.0f, 0, 1.0f, -120))));
    PanelLeaf* viewportPanel = static_cast<PanelLeaf*>(container->getPanel());
    viewportPanel->addChild<Viewport>(UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, 0, 1.0f, 0))));
    PanelSplit* split1 = container->splitPanel(PanelSplitDirection::Vertical, 0.8f, PanelSplitPlacement::First);
    PanelSplit* split2 = split1->splitPanel(PanelSplitPlacement::First, PanelSplitDirection::Horizontal, 0.2f, PanelSplitPlacement::Second);
    PanelSplit* split3 = split1->splitPanel(PanelSplitPlacement::Second, PanelSplitDirection::Horizontal, 0.5f, PanelSplitPlacement::First);
    PanelLeaf* consolePanel = static_cast<PanelLeaf*>(split3->getPanel(PanelSplitPlacement::First));
    consolePanel->addChild<Console>(UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, 0, 1.0f, 0))));

    TabBar* tabBar = window->addWidget<TabBar>(UILayout(UIRect(UIDim2(0.0f, 10, 1.0f, -90), UIDim2(1.0f, -20, 0.0f, 50))), TabBarDirection::Horizontal);
    TabId tab1 = tabBar->addTab("Tab1");
    TabId tab2 = tabBar->addTab("Tab2");

    App::get().run();

    return 0;
}