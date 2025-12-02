#include "window_impl.hpp"

WindowImpl::WindowImpl(App& app, const u32& i, const WindowConfig& c) : app(app), id(i), config(c) {}

const u32 WindowImpl::getId() {
    return id;
}

WindowConfig WindowImpl::getConfig() {
    return config;
}