#pragma once

#include <datatypes/vector2.hpp>

struct Rect {
    Vector2 position, size;

    Rect() : position(Vector2()), size(Vector2()) {}
    Rect(Vector2 position, Vector2 size) : position(position), size(size) {}
    Rect(float positionX, float positionY, float sizeX, float sizeY) : position(Vector2(positionX, positionY)), size(Vector2(sizeX, sizeY)) {}
    Rect(int positionX, int positionY, int sizeX, int sizeY) : position(Vector2(positionX, positionY)), size(Vector2(sizeX, sizeY)) {}

    bool operator==(const Rect& other) const {
        return position == other.position && size == other.size;
    }

    bool operator!=(const Rect& other) const {
        return position != other.position || size != other.size;
    }
};