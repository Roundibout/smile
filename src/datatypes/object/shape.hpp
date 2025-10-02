#pragma once

#include <cstdint>
#include <vector>

using Id = uint32_t;

struct Shape {
    Id id;

    Shape(Id id) : id(id) {}
};