#pragma once

#include <string>

struct ContextActionConfig {
    std::string name = "Context Action";
};

class ContextAction {
private:
    ContextActionConfig config;
public:
    ContextAction(ContextActionConfig config);
};