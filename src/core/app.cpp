/*
File:
    app.cpp
Authors:
    Lucas
Purpose:
    Implementation of the App singleton, used to control the C++ side functionality of the program.
License:
    MIT (see LICENSE file)
*/

#include "app.hpp"

Window* App::createWindow(const WindowConfig& config) {
    Logger::print("Creating window");

    // Create window and associate it with the next window id
    std::unique_ptr<Window> window = std::make_unique<Window>(*this, nextId, config);
    Window* ptr = window.get();
    windows[nextId] = std::move(window);

    // Increment window id
    nextId++;

    return ptr;
}

Window* App::getWindowById(const uint32_t& id) {
    auto it = windows.find(id);
    if (it != windows.end()) {
        return it->second.get(); // Found it
    } else {
        return nullptr; // Doesn't exist
    }
}

void App::destroyWindow(const uint32_t& id) {
    auto it = windows.find(id);
    if (it != windows.end()) {
        windows.erase(it);
    }
}

void App::setUIScale(float scale) {
    uiScale = std::clamp(scale, 0.5f, 2.0f);
    needsRender = true;
}

void App::step() {
    // Get clock
    using clock = std::chrono::steady_clock;

    // Get current time
    std::chrono::time_point<clock> currentTime = clock::now();
    std::chrono::duration<float> elapsed = currentTime - previousTime; // Get how much time has passed
    previousTime = currentTime; // Set previous time to now

    float deltaTime = elapsed.count(); // seconds

    // Update all windows
    for (auto& [id, window] : windows) {
        window->update(deltaTime);
    }

    // Render all windows
    for (auto& [id, window] : windows) {
        if (needsRender == true) {
            window->renderer.dirty();
        }
        window->render(uiScale);
    }
    needsRender = false;
}

void App::run() {
    // Stop if the app is already running
    if (running == true) return;

    running = true;

    while (running) {
        // Process all windows' native windows (can lead to a freeze on Windows when moving and resizing, is fixed internally)
        for (auto& [id, window] : windows) {
            window->process();
        }

        step();

        // TEMP
        // Quit app once there are no windows
        if (windows.empty()) {
            quit();
        }
    }
}

void App::quit() {
    running = false;
}