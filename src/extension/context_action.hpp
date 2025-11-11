#pragma once

#include <string>

struct ContextActionDefinition {
    std::string name = "Context Action";
};

class ContextAction {
private:
    std::string name;
public:
    ContextAction(ContextActionDefinition definition);

    std::string getName() {return name;}
};