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

#include <sol/sol.hpp>

#include <iostream>
#include <memory>

#include <app/app.hpp>

extern void register_bindings(sol::state& lua);

int main(int argc, char *argv[]) {
    std::cout << "Initializing smile..." << std::endl;
    
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

    std::cout << std::endl << "---- Starting smile ----" << std::endl << std::endl;

    // Run main Lua entry point
    auto result = lua.safe_script_file("scripts/main.lua", sol::script_pass_on_error);
    if (!result.valid()) {
        sol::error err = result;

        std::cout << std::endl << "---- Smile crashed -----" << std::endl;
        std::cerr << std::endl << "[ ERROR ] " << std::endl << err.what() << std::endl;
        
        std::cout << std::endl << "Press enter to exit...";
        std::cin.get();

        return -1; // Lua error
    } else {
        // Valid response
    }

    std::cout << std::endl << "------ Smile quit ------" << std::endl;
    std::cout << std::endl << "Press enter to exit...";
    std::cin.get();

    return 0;
}