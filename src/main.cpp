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

#include <sol/sol.hpp>

#include <app/app.hpp>
#include <util/logger.hpp>

extern void register_bindings(sol::state& lua);

int main(int argc, char *argv[]) {
    Logger::print("Initializing smile...");
    
    sol::state lua;
    lua.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::string,
        sol::lib::table,
        sol::lib::math
    );

    // Bind usertypes
    register_bindings(lua);

    // Run Lua

    // Run a script to set the package path to the scripts folder
    lua.safe_script("package.path = package.path..';./scripts/?.lua'");

    Logger::print("\n---- Starting smile ----\n");

    // Run main Lua entry point
    auto result = lua.safe_script_file("scripts/main.lua", sol::script_pass_on_error);
    if (!result.valid()) {
        sol::error err = result;

        Logger::print("\n---- Smile crashed -----");
        Logger::print("\n[ ERROR ]\n" + std::string(err.what()));
        
        return -1; // Lua error
    } else {
        // Valid response
    }

    return 0;
}