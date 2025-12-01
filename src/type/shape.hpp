#pragma once

import numbers;

#include <vector>

struct Shape {
    using Id = u32;

    Id id;

    Shape(Id id) : id(id) {}
};