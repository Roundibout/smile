/*
File:
    window.hpp
Authors:
    Lucas
Purpose:
    Definition of the Window class, used to create windows in Smile
License:
    MIT (see LICENSE file)
*/

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>
#include <queue>
#include <mutex>

#include <sol/sol.hpp>

#include <datatypes/vector2.hpp>
#include <window/window_input.hpp>

#include <window/window_manager.hpp>

enum class WindowEvent {
    Update,
    Render,
    Input
};

class Window {
private:
    uint32_t id;
    std::unordered_map<WindowEvent, std::vector<sol::function>> callbacks;

    std::queue<WindowInput> inputs;
    std::mutex inputMutex;
public:
    Window(const std::string& title, const Vector2& size);
    void pushInput(WindowInput input);
    void processInputs();
    void update(float deltaTime);
    void render();
    void connectCallback(WindowEvent event, sol::function callback);
    uint32_t getId() {return id;}
};