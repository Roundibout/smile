#pragma once

#include <datatypes/ui_types.hpp>

#include <render/renderer.hpp>

#include <ui/theme.hpp>
#include <ui/ui_tools.hpp>

class Window; // Forward declaration (we don't need the actual window class)

class Widget {
public:
    UILayout layout;
    Window* window;
    
    Widget(Window* window, UILayout layout) : window(window), layout(layout) {}
    virtual ~Widget() = default;

    virtual void update(float deltaTime, const UIBounds& bounds) = 0;
    virtual void render(const UIBounds& bounds) = 0;

    virtual bool processWindowInput(WindowInput& input, const UIBounds& bounds) = 0;
    virtual void observeWindowInput(WindowInput& input, const UIBounds& bounds) = 0;
};