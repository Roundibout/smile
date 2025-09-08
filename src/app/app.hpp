/*
File:
    app.hpp
Authors:
    Lucas
Purpose:
    Definition of the App singleton, used to control the C++ side functionality of the program.
License:
    MIT (see LICENSE file)
*/

#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <chrono>
#include <thread>

#include <datatypes/vector2.hpp>
#include <window/window.hpp>

class Window; // prevent cycle

class App {
private:
    // Private constructor to prevent direct instantiation (there should be only one app)
    App() {
        std::cout << "App created." << std::endl;
    }
    // Delete copy constructor and assignment operator to prevent copying (same reason)
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    bool running = false;
    int fps = 60;
    std::chrono::time_point<std::chrono::steady_clock> previousTime = std::chrono::steady_clock::now();

    std::unordered_map<uint32_t, std::shared_ptr<Window>> windows;
    uint32_t nextId = 1;
public:
    static App& get() {
        // Initialize app only once
        static App instance; 
        return instance;
    }

    std::shared_ptr<Window> createWindow(const std::string& title, const Vector2& size);

    std::shared_ptr<Window> getWindowById(const uint32_t& id);

    std::chrono::time_point<std::chrono::steady_clock> step();

    void run();
    void quit();
};