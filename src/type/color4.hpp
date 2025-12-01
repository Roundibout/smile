#pragma once

#include <string>
#include <algorithm>

struct Color4 {
    float r, g, b, a = 1.0f;

    Color4() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {};
    Color4(float r, float g, float b) : 
        r(std::clamp(r, 0.0f, 1.0f)), 
        g(std::clamp(g, 0.0f, 1.0f)), 
        b(std::clamp(b, 0.0f, 1.0f)) 
    {};
    Color4(float r, float g, float b, float a) : 
        r(std::clamp(r, 0.0f, 1.0f)), 
        g(std::clamp(g, 0.0f, 1.0f)), 
        b(std::clamp(b, 0.0f, 1.0f)), 
        a(std::clamp(a, 0.0f, 1.0f)) 
    {};

    std::string toString() const;
};