#pragma once

#include <cstdint>
#include <vector>

#include <datatypes/object/triangle.hpp>

using Id = uint32_t;

struct Face {
    Id id;
    std::vector<Id> edges; // Edge ids
    std::vector<Id> vertices; // Vertex ids
    std::vector<Id> holes; // Other face ids

    std::vector<Triangle> triangles;

    Face(Id id) : id(id) {}
};