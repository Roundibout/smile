#pragma once

#include <cstdint>
#include <vector>
#include <datatypes/object/vertex.hpp>

struct Triangle {
    Vertex::Id vertex1, vertex2, vertex3;

    Triangle(Vertex::Id vertex1, Vertex::Id vertex2, Vertex::Id vertex3) : vertex1(vertex1), vertex2(vertex2), vertex3(vertex3) {}
};