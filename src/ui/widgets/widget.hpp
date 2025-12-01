#pragma once

#include <type/ui_types.hpp>

#include <render/renderer.hpp>

#include <ui/theme.hpp>
#include <ui/ui_tools.hpp>

class App;
class Window;

class Widget {
public:
    App& app;
    Window* window;
    UILayout layout;
    
    Widget(App& app, Window* window, UILayout layout) : app(app), window(window), layout(layout) {}
    virtual ~Widget() = default;

    virtual void update(float deltaTime, const UIBounds& bounds) = 0;
    virtual void render(const UIBounds& bounds) = 0;

    virtual bool processWindowInput(WindowInput& input, const UIBounds& bounds) = 0;
    virtual void observeWindowInput(WindowInput& input, const UIBounds& bounds) = 0;
};