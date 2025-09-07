/*
File:
    vector2.hpp
Authors:
    Lucas
Purpose:
    Definition of the Vector2 datatype
License:
    MIT (see LICENSE file)
*/

#pragma once

#include <string>
#include <cmath>

struct Vector2 {
    float x, y;

    Vector2() : x(0), y(0) {}
    Vector2(int x, int y) : x(float(x)), y(float(y)) {}
    Vector2(float x, float y) : x(x), y(y) {}
    
    Vector2 operator+(const Vector2& other) const {
        return Vector2{x + other.x, y + other.y};
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2{x - other.x, y - other.y};
    }

    Vector2 operator*(float scalar) const {
        return Vector2{x * scalar, y * scalar};
    }

    Vector2 operator/(float scalar) const {
        return Vector2{x / scalar, y / scalar};
    }

    Vector2 idiv(float scalar) const {
        return Vector2{std::floor(x / scalar), std::floor(y / scalar)};
    }

    Vector2 operator%(float scalar) const {
        return Vector2{std::fmod(x, scalar), std::fmod(y, scalar)};
    }

    Vector2 pow(float exponent) const {
        return Vector2{std::pow(x, exponent), std::pow(y, exponent)};
    }

    Vector2 operator-() const {
        return Vector2{-x, -y};
    }

    bool operator==(const Vector2& other) const {
        return x == other.x && y == other.y;
    }

    std::string to_string() const;
};