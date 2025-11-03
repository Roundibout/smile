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

    App::get().createWindow(config);
    App::get().run();

    return 0;
}