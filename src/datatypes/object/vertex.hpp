#pragma once

#include <cstdint>

#include <datatypes/vector2.hpp>

using Id = uint32_t;

struct Vertex {
    Id id;
    float x, y = 0.0f;

    Vertex(Id id) : id(id) {};
    Vertex(Id id, const Vector2& position) : id(id), x(position.x), y(position.y) {};
    Vertex(Id id, float x, float y) : id(id), x(x), y(y) {}
};