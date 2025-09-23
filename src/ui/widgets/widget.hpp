#pragma once

#include <datatypes/ui_types.hpp>

#include <render/renderer.hpp>

#include <ui/theme.hpp>
#include <ui/ui_tools.hpp>

class Window; // Forward declaration (we don't need the actual window class)

class Widget {
protected:
    UILayout layout;
    Window* window;
public:
    Widget(Window* window, UILayout layout) : window(window), layout(layout) {}
    virtual ~Widget() = default;

    virtual void update(float deltaTime) = 0;
    virtual void render(UIBounds bounds) = 0;

    virtual void processWindowInput(WindowInput& input) = 0;
};