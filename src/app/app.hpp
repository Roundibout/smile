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
#include <vector>
#include <memory>
#include <chrono>
#include <thread>

#include <datatypes/vector2.hpp>
#include <window/window.hpp>

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

    std::vector<std::shared_ptr<Window>> windows;
public:
    static App& get() {
        // Initialize app only once
        static App instance; 
        return instance;
    }

    std::shared_ptr<Window> createWindow(const std::string& title, const Vector2& size);

    void run();
    void pause();
};