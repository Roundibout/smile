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

void App::run() {
    // Stop if the app is already running
    if (running == true) {return;}

    using clock = std::chrono::steady_clock;

    running = true;

    std::chrono::milliseconds frameDuration(1000 / fps);

    auto previousTime = clock::now();

    while (running) {
        auto currentTime = clock::now();
        std::chrono::duration<float> elapsed = currentTime - previousTime;
        previousTime = currentTime;

        float deltaTime = elapsed.count(); // seconds

        // Process all windows' inputs
        for (auto& [id, window] : windows) {
            window->process();
        }

        // Update all windows
        for (auto& [id, window] : windows) {
            window->update(deltaTime);
        }

        // Render all windows (in the future only render if something changed, returned by update)
        for (auto& [id, window] : windows) {
            window->render();
        }

        // Sleep to cap at 60 fps
        auto workTime = clock::now() - currentTime;
        if (workTime < frameDuration) {
            std::this_thread::sleep_for(frameDuration - workTime); // stop the CPU from busymaxxing
        }
    }
}

void App::quit() {
    running = false;
}