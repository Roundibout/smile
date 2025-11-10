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

#include <core/app.hpp>
#include <extension/extension_manager.hpp>
#include <window/main_window.hpp>
#include <util/logger.hpp>

int main(int argc, char *argv[]) {
    Logger::print("//// Initializing ////\n");

    App app;
    
    Logger::print("//// Loading extensions ////\n");

    app.extensionManager->loadAll();

    Logger::print("\n//// Starting smile ////\n");

    app.mainWindow->create();
    app.run();

    return 0;
}