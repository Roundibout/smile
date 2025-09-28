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

#include <ui/widgets/button.hpp>

#include <cstdlib>
#include <ctime>

Window::Window(const uint32_t& id, const WindowConfig& config)
    #ifdef _WIN32
        : impl(std::make_unique<WindowWin32>(id, config)), // initializer shenanigans
        renderer(impl.get()),
        lastSize(config.size)
    #endif
{
    // maybe do other stuff later
    std::srand(std::time(nullptr));
    for (int i = 1; i < 100; i++) {
        float x = static_cast<float>(std::rand()) / RAND_MAX;
        float y = static_cast<float>(std::rand()) / RAND_MAX;
        UILayout layout = UILayout(UIRect(UIDim2(x, 0, y, 0), UIDim2(0.0f, 100, 0.0f, 50)));
        layout.cornerRT = UIDim(static_cast<float>(std::rand()) / RAND_MAX, 0);
        layout.cornerLT = UIDim(static_cast<float>(std::rand()) / RAND_MAX, 0);
        layout.cornerRB = UIDim(static_cast<float>(std::rand()) / RAND_MAX, 0);
        layout.cornerLB = UIDim(static_cast<float>(std::rand()) / RAND_MAX, 0);
        addWidget<Button>(layout);
    }
}

void Window::process() {
    // Clean renderer
    renderer.clean();
    bool overdirtied = false;

    // Get inputs through impl update
    inputs = impl->update();

    UIBounds bounds(impl->getSize(), UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, 0, 1.0f, 0))));

    // Process inputs
    while (!inputs.empty()) {
        WindowInput& input = inputs.front();

        // Process input on widgets first
        bool consumed = false;
        for (auto it = widgets.rbegin(); it != widgets.rend(); ++it) {
            if (consumed == false) {
                consumed = (*it)->processWindowInput(input, bounds);
            } else {
                (*it)->observeWindowInput(input, bounds);
            }
        }

        // Find any input callbacks
        auto it = callbacks.find(WindowEvent::Input);
        if (it != callbacks.end()) { // Is there any?
            // Go through all of them and run them
            for (auto& func : it->second) {
                func(input);
            }
        }

        // Set dirty overrides
        if (input.type == WindowInputType::WindowResized || input.type == WindowInputType::WindowMoved) {
            overdirtied = true;
            lastSize = impl->getSize();
        }

        inputs.pop(); // remove the input
    }

    if (overdirtied == true) {
        renderer.dirty();
    }
}

void Window::update(float deltaTime) {
    // Check for new window size and force render
    Vector2 windowSize = impl->getSize();
    if (lastSize != windowSize) {
        renderer.dirty();
        lastSize = windowSize;
    }
    // Update widgets first
    UIBounds bounds(impl->getSize(), UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, 0, 1.0f, 0))));
    for (auto& widget : widgets) {
        widget->update(deltaTime, bounds);
    }
    // Find any update callbacks
    auto it = callbacks.find(WindowEvent::Update);
    if (it != callbacks.end()) { // Is there any?
        // Go through all of them and run them
        for (auto& func : it->second) {
            func(deltaTime);
        }
    }
}

void Window::render(float uiScale) {
    if (renderer.isDirty() || blank == true) { // Only render if the renderer is dirty or the window is blank
        blank = false;
        // Begin the frame
        renderer.beginFrame(uiScale);
        // Render widgets first
        UIBounds bounds(impl->getSize(), UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, 0, 1.0f, 0))));
        for (auto& widget : widgets) {
            widget->render(bounds);
        }
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
}

void Window::connectCallback(WindowEvent event, sol::function callback) {
    callbacks[event].push_back(callback);
}