#pragma once

import numbers;
#include "type/vector2.hpp"

struct Point {
    using Id = u32;

    Id id;
    float x, y = 0.0f;

    Point(Id id) : id(id) {};
    Point(Id id, const Vector2& position) : id(id), x(position.x), y(position.y) {};
    Point(Id id, float x, float y) : id(id), x(x), y(y) {}
};