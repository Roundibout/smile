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
    std::shared_ptr<Window> win = std::make_shared<Window>(title, size);
    windows.push_back(win);
    return win;
}

void App::run() {
    using clock = std::chrono::steady_clock;

    running = true;

    std::chrono::milliseconds frameDuration(1000 / fps);

    auto previousTime = clock::now();

    while (running) {
        auto currentTime = clock::now();
        std::chrono::duration<float> elapsed = currentTime - previousTime;
        previousTime = currentTime;

        float deltaTime = elapsed.count(); // seconds

        // Update all windows
        for (auto& window : windows) {
            window->update(deltaTime);
        }

        // Sleep to cap at 60 fps
        auto workTime = clock::now() - currentTime;
        if (workTime < frameDuration) {
            std::this_thread::sleep_for(frameDuration - workTime); // stop the CPU from busymaxxing
        }
    }
}

void App::pause() {
    running = false;
}