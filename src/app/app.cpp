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

}