#include "window_impl.hpp"

WindowImpl::WindowImpl(App& app, const uint32_t& i, const WindowConfig& c) : app(app), id(i), config(c) {}

const uint32_t WindowImpl::getId() {
    return id;
}

WindowConfig WindowImpl::getConfig() {
    return config;
}