#pragma once

#include <string>

struct MenuActionConfig {
    std::string name = "Menu Action";
};

class MenuAction {
private:
    MenuActionConfig config;
public:
    MenuAction(MenuActionConfig config);
};