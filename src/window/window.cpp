/*
File:
    window.cpp
Authors:
    Lucas
Purpose:
    Implementation of the Window class, used to create windows in Smile
License:
    MIT (see LICENSE file)
*/

#include "window.hpp"

Window::Window(const uint32_t& id, const std::string& title, const Vector2& size)
    #ifdef _WIN32
        : impl(std::make_unique<WindowWin32>(id, title, size)), // initializer shenanigans
        renderer(impl.get())
    #endif
{
    // maybe do other stuff later
}

void Window::process() {
    // Get inputs through impl update
    inputs = impl->update();

    // Process inputs
    while (!inputs.empty()) {
        WindowInput& input = inputs.front();

        // Find any input callbacks
        auto it = callbacks.find(WindowEvent::Input);
        if (it != callbacks.end()) { // Is there any?
            // Go through all of them and run them
            for (auto& func : it->second) {
                func(input);
            }
        }

        inputs.pop(); // remove the input
    }
}

void Window::update(float deltaTime) {
    // Find any update callbacks
    auto it = callbacks.find(WindowEvent::Update);
    if (it != callbacks.end()) { // Is there any?
        // Go through all of them and run them
        for (auto& func : it->second) {
            func(deltaTime);
        }
    }
}

void Window::render() {
    // Begin the frame
    renderer.beginFrame();
    // Find any render callbacks
    auto it = callbacks.find(WindowEvent::Render);
    if (it != callbacks.end()) { // Is there any?
        // Go through all of them and run them
        for (auto& func : it->second) {
            func();
        }
    }
    // End the frame
    renderer.endFrame();
}

void Window::connectCallback(WindowEvent event, sol::function callback) {
    callbacks[event].push_back(callback);
}