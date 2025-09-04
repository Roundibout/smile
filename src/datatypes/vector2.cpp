/*
File:
    vector2.cpp
Authors:
    Lucas
Purpose:
    Implementation of the Vector2 datatype
License:
    MIT (see LICENSE file)
*/

#include "vector2.hpp"

std::string Vector2::to_string() const {
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