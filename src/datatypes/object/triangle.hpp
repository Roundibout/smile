#pragma once

#include <cstdint>
#include <vector>

using Id = uint32_t;

struct Triangle {
    Id vertex1, vertex2, vertex3;

    Triangle(Id vertex1, Id vertex2, Id vertex3) : vertex1(vertex1), vertex2(vertex2), vertex3(vertex3) {}
};