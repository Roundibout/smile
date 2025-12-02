#pragma once

#include <string>
#include <cmath>

struct Vector2 {
    float x, y = 0.0f;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(
        int x, 
        int y
    ) : 
        x(static_cast<float>(x)), 
        y(static_cast<float>(y)) 
    {}
    Vector2(
        float x, 
        float y
    ) : 
        x(x), 
        y(y) 
    {}
    Vector2(
        double x, 
        double y
    ) : 
        x(static_cast<float>(x)), 
        y(static_cast<float>(y)) 
    {}
    
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
        return x == other.x and y == other.y;
    }

    bool operator!=(const Vector2& other) const {
        return x != other.x or y != other.y;
    }

    Vector2& operator+=(const Vector2& other) {
        x += other.x; 
        y += other.y; 
        return *this;
    }

    Vector2& operator-=(const Vector2& other) {
        x -= other.x; 
        y -= other.y; 
        return *this;
    }

    Vector2& operator*=(float scalar) {
        x *= scalar; 
        y *= scalar; 
        return *this;
    }

    Vector2& operator/=(float scalar) {
        x /= scalar; 
        y /= scalar; 
        return *this;
    }

    static float dot(const Vector2& a, const Vector2& b) {
        return a.x * b.x + a.y * b.y;
    }

    static float cross(const Vector2& a, const Vector2& b) {
        return a.x * b.y - a.y * b.x; // 2D cross product magnitude
    }

    static float length(const Vector2& v) {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    static Vector2 normalize(const Vector2& v) {
        float len = length(v);
        if (len == 0.0f) return Vector2(0.0f, 0.0f);
        return v / len;
    }

    static float angleOf(const Vector2& v) {
        return atan2(v.y, v.x);
    }

    std::string toString() const {
        std::string xString, yString;

        if (std::fmod(x, 1.0f) != 0.0f) {
            xString = std::to_string(x);
        } else {
            xString = std::to_string(int(x));
        }
        if (std::fmod(y, 1.0f) != 0.0f) {
            yString = std::to_string(y);
        } else {
            yString = std::to_string(int(y));
        }

        return "(" + xString + ", " + yString + ")";
    }
};