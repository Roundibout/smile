#pragma once

#include <cstdint>

using Id = uint32_t;

struct Edge {
    Id id;
    Id vertex1, vertex2;

    Edge(Id id, Id vertex1, Id vertex2) : id(id), vertex1(vertex1), vertex2(vertex2) {}
};