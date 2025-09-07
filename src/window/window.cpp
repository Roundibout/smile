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
    // Create window through window manager, it will give us an id back
    id = WindowManager::get().createWindow(title, size);
}

void Window::pushInput(WindowInput input) {
    // Called from WindowManager
    std::lock_guard<std::mutex> lock(inputMutex);
    inputs.push(std::move(input));
}

void Window::processInputs() {
    std::queue<WindowInput> localQueue;
    { // again, create a scope so lock is destroyed automatically
        std::lock_guard<std::mutex> lock(inputMutex);
        std::swap(localQueue, inputs);  // take all inputs atomically
    }

    while (!localQueue.empty()) {
        WindowInput& input = localQueue.front();

        // Find any input callbacks
        auto it = callbacks.find(WindowEvent::Input);
        if (it != callbacks.end()) { // Is there any?
            // Go through all of them and run them
            for (auto& func : it->second) {
                func(input);
            }
        }

        localQueue.pop(); // remove the input
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
    // Find any render callbacks
    auto it = callbacks.find(WindowEvent::Render);
    if (it != callbacks.end()) { // Is there any?
        // Go through all of them and run them
        for (auto& func : it->second) {
            func();
        }
    }
}

void Window::connectCallback(WindowEvent event, sol::function callback) {
    callbacks[event].push_back(callback);
}