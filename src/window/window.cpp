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

Window::Window(const std::string& title, const Vector2& size) {
    std::cout << "title: " << title << ", size: " << size.to_string() << std::endl;

    id = WindowManager::get().createWindow(title, size);
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

void Window::connectCallback(WindowEvent event, sol::function callback) {
    callbacks[event].push_back(callback);
}