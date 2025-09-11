/*
File:
    color4.hpp
Authors:
    Lucas
Purpose:
    Definition of the Color4 datatype
License:
    MIT (see LICENSE file)
*/

#pragma once

#include <algorithm>

struct Color4 {
    float r, g, b, a;

    Color4();
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
};