#pragma once

#include "type/numbers.hpp"

#include <vector>

struct Shape {
    using Id = u32;

    Id id;

    Shape(Id id) : id(id) {}
};