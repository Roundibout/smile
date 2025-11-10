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

WindowImpl::WindowImpl(App& app, const uint32_t& i, const WindowConfig& c) : app(app), id(i), config(c) {}

const uint32_t WindowImpl::getId() {
    return id;
}

WindowConfig WindowImpl::getConfig() {
    return config;
}