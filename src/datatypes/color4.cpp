/*
File:
    color4.cpp
Authors:
    Lucas
Purpose:
    Implementation of the Color4 datatype
License:
    MIT (see LICENSE file)
*/

#include "color4.hpp"

std::string Color4::toString() const {
    return "(" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", " + std::to_string(a) + ")";
}