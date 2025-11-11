#pragma once

#include <string>

struct MenuActionDefinition {
    std::string name = "Menu Action";
};

class MenuAction {
private:
    std::string name;
public:
    MenuAction(MenuActionDefinition definition);

    std::string getName() {return name;}
};