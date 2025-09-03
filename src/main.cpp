#include <sol/sol.hpp>

#include <iostream>

int main() {
    std::cout << "Initializing Lua environment" << std::endl;
    
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    auto result = lua.safe_script_file("scripts/main.lua", sol::script_pass_on_error);
    if (!result.valid()) {
        std::cerr << "Error loading script" << std::endl;
        return -1;
    }

    std::cin.get();

    return 0;
}