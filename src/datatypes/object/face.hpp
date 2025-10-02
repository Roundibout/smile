#pragma once

#include <cstdint>
#include <vector>

using Id = uint32_t;

struct Face {
    Id id;
    std::vector<Id> edges;
    std::vector<Id> holes;

    Face(Id id) : id(id) {}
};