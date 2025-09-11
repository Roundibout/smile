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

WindowImpl::WindowImpl(const uint32_t& i, const std::string& t, const Vector2& s) : id(i), title(t), size(s) {}

const uint32_t WindowImpl::getId() {
    return id;
}