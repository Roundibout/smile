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

#include <datatypes/vector2.hpp>
#include <app/app.hpp>

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

    // ---- BIND USERTYPES ---- //

    // -- Datatypes -- //

    // Vector2
    lua.new_usertype<Vector2>("Vector2",
        // Constructors
        sol::constructors<
            Vector2(), 
            Vector2(float, float)
        >(),
        // Fields
        "x", &Vector2::x,
        "y", &Vector2::y,
        // Operators
        sol::meta_function::addition, &Vector2::operator+,
        sol::meta_function::subtraction, sol::resolve<Vector2(const Vector2&) const>(&Vector2::operator-),
        sol::meta_function::multiplication, &Vector2::operator*,
        sol::meta_function::division, &Vector2::operator/,
        sol::meta_function::floor_division, &Vector2::idiv,
        sol::meta_function::modulus, &Vector2::operator%,
        sol::meta_function::power_of, &Vector2::pow,
        sol::meta_function::equal_to, &Vector2::operator==,
        sol::meta_function::unary_minus, sol::resolve<Vector2() const>(&Vector2::operator-)
    );

    // -- Classes -- //

    // -- App -- //
    lua.new_usertype<App>("App",
        "Run", &App::run,

        "CreateWindow", &App::createWindow
    );
    // Assign the singleton app to a global variable so you don't need use the get function
    lua["App"] = &App::get();

    // ---- RUN LUA ---- //

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

        return -1; // Lua error (unknown)
    } else {
        // Valid response
    }

    std::cout << std::endl << "------ Smile quit ------" << std::endl;
    std::cout << std::endl << "Press enter to exit...";
    std::cin.get();

    return 0;
}