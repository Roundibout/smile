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

std::shared_ptr<Window> App::createWindow(const std::string& title, const Vector2& size) {
    std::cout << "Creating window" << std::endl;

    // Create window and associate it with the next window id
    std::shared_ptr<Window> window = std::make_shared<Window>(title, size);
    windows[nextId] = window;

    // Increment window id
    nextId++;

    return window;
}

std::shared_ptr<Window> App::getWindowById(const uint32_t& id) {
    auto it = windows.find(id);
    if (it != windows.end()) {
        return it->second; // Found it
    } else {
        return nullptr; // Doesn't exist
    }
}

std::chrono::time_point<std::chrono::steady_clock> App::step() {
    // Get clock
    using clock = std::chrono::steady_clock;

    // Get current time
    std::chrono::time_point<std::chrono::steady_clock> currentTime = clock::now();
    std::chrono::duration<float> elapsed = currentTime - previousTime; // Get how much time has passed
    previousTime = currentTime; // Set previous time to now

    float deltaTime = elapsed.count(); // seconds

    // Update all windows
    for (auto& [id, window] : windows) {
        window->update(deltaTime);
    }

    // Render all windows (in the future only render if something changed, returned by update)
    for (auto& [id, window] : windows) {
        window->render();
    }

    return currentTime; // Return time for use in main loop
}

void App::run() {
    // Stop if the app is already running
    if (running == true) {return;}

    running = true;

    using clock = std::chrono::steady_clock;
    std::chrono::milliseconds frameDuration(1000 / fps);
    previousTime = clock::now();

    while (running) {
        // Process all windows' native windows (can lead to a freeze on Win32 when moving and resizing, is fixed internally)
        for (auto& [id, window] : windows) {
            window->process();
        }

        auto currentTime = App::get().step();

        // Sleep to cap at set fps
        auto workTime = clock::now() - currentTime;
        if (workTime < frameDuration) {
            std::this_thread::sleep_for(frameDuration - workTime); // stop the CPU from busymaxxing
        }
    }
}

void App::quit() {
    running = false;
}