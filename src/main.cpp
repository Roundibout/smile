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

#include <datatypes/vector2.hpp>
#include <app/app.hpp>
#include <window/window.hpp>
#include <window/window_input.hpp>

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

    // -- Enums -- //

    lua.new_enum<WindowEvent>("WindowEvent",
        {
            {"Update", WindowEvent::Update},
            {"Render", WindowEvent::Render},
            {"Input", WindowEvent::Input}
        }
    );

    // Window Input

    lua.new_enum<WindowInputType>("WindowInputType",
        {
            {"MouseButtonDown", WindowInputType::MouseButtonDown},
            {"MouseButtonUp", WindowInputType::MouseButtonUp},
            {"MouseMove", WindowInputType::MouseMove},
            {"MouseScroll", WindowInputType::MouseScroll},
            {"KeyDown", WindowInputType::KeyDown},
            {"KeyUp", WindowInputType::KeyUp},
            {"WindowResized", WindowInputType::WindowResized},
            {"WindowClosed", WindowInputType::WindowClosed}
        }
    );

    lua.new_enum<MouseButton>("MouseButton",
        {
            {"Left", MouseButton::Left},
            {"Right", MouseButton::Right},
            {"Middle", MouseButton::Middle}
        }
    );

    // Use macro to easily copy over the same items for the enum
    lua.new_enum<KeyCode>("KeyCode", {
    #define X(name) std::make_pair(#name, KeyCode::name),
        KEY_CODE_LIST
    #undef X
    });

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

    // Window Input
    lua.new_usertype<MouseInput>("MouseInput",
        "button", &MouseInput::button,
        "position", sol::property(
            [](MouseInput& self) -> Vector2& {return self.position;},
            [](MouseInput& self, Vector2 position) {self.position = position;}
        ), // nested struct (with setter)
        "scroll", sol::property(
            [](MouseInput& self) -> Vector2& {return self.scroll;},
            [](MouseInput& self, Vector2 scroll) {self.scroll = scroll;}
        ) // same thing
    );

    lua.new_usertype<WindowInput>("WindowInput",
        "type", &WindowInput::type,
        "mouse", sol::property([](WindowInput& self) -> MouseInput& {return self.mouse;}), // nested struct
        "key", &WindowInput::key
    );

    // -- Classes -- //

    lua.new_usertype<Window>("Window",
        "renderer", sol::property([](Window& self) -> Renderer& {return self.renderer;}), // nested struct

        "ConnectUpdate", [](Window& self, sol::function callback){self.connectCallback(WindowEvent::Update, callback);},
        "ConnectRender", [](Window& self, sol::function callback){self.connectCallback(WindowEvent::Render, callback);},
        "ConnectInput", [](Window& self, sol::function callback){self.connectCallback(WindowEvent::Input, callback);}
    );

    // -- App -- //
    lua.new_usertype<App>("App",
        "Run", &App::run,
        "Quit", &App::quit,

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