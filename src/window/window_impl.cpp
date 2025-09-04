/*
File:
    window_impl.cpp
Authors:
    Lucas
Purpose:
    Implementation of the common Window implementation class
License:
    MIT (see LICENSE file)
*/

#include "window_impl.hpp"

WindowImpl::WindowImpl(const std::string& t, const Vector2& s) : title(t), size(s) {}