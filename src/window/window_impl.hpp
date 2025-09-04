/*
File:
    window_impl.hpp
Authors:
    Lucas
Purpose:
    Definition of the common Window implementation interface
License:
    MIT (see LICENSE file)
*/

#pragma once

#include <string>

#include <datatypes/vector2.hpp>

class WindowImpl {
protected:
    std::string title;
    Vector2 size;
public:
    WindowImpl(const std::string& t, const Vector2& s);
};