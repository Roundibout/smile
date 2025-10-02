#pragma once

#include <vector>

#include <core/instance.hpp>

class Document {
private:
    std::vector<Instance> instances;
public:
    Document();
};