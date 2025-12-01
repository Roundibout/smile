#pragma once

import numbers;
#include "type/color4.hpp"

struct Line {
    using Id = u32;
    
    Id id;
    Id point1, point2;

    Line(Id id, Id point1, Id point2) : id(id), point1(point1), point2(point2) {}
};