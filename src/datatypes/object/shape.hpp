#pragma once

#include <cstdint>
#include <vector>

struct Shape {
    using Id = uint32_t;

    Id id;

    Shape(Id id) : id(id) {}
};