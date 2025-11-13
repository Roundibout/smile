#pragma once

#include <string>

class Instance {
protected:
    std::string name;
public:
    Instance(std::string name) : name(name) {}
    virtual ~Instance() = default;

    std::string getName() {return name;}
};