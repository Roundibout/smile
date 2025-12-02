#pragma once

#include <vector>

#include "type/numbers.hpp"
#include "type/vector2.hpp"
#include "type/color4.hpp"

struct Point {
    using Id = u32;

    Id id;
    float x, y = 0.0f;

    Point(Id id) : id(id) {};
    Point(Id id, 
        const Vector2& position
    ) : 
        id(id), 
        x(position.x), 
        y(position.y) 
    {};
    Point(
        Id id, 
        float x, 
        float y
    ) : 
        id(id), 
        x(x), 
        y(y) 
    {}
};

struct Line {
    using Id = u32;
    
    Id id;
    Id point1, point2;

    Line(
        Id id, 
        Id point1, 
        Id point2
    ) : 
        id(id), 
        point1(point1), 
        point2(point2) 
    {}
};

struct Shape {
    using Id = u32;

    Id id;

    Shape(Id id) : id(id) {}
};