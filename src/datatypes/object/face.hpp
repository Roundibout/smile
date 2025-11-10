#pragma once

#include <cstdint>
#include <vector>

#include <datatypes/object/triangle.hpp>

struct Face {
    using Id = uint32_t;

    Id id;
    std::vector<Id> edges; // Edge ids
    std::vector<Id> vertices; // Vertex ids
    std::vector<Id> holes; // Other face ids

    std::vector<Triangle> triangles;

    Face(Id id) : id(id) {}
};