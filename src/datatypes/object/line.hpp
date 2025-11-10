#pragma once

#include <cstdint>

#include <datatypes/color4.hpp>

struct Line {
    using Id = uint32_t;
    
    Id id;
    Id point1, point2;

    Line(Id id, Id point1, Id point2) : id(id), point1(point1), point2(point2) {}
};