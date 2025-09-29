#pragma once

#include <datatypes/ui_types.hpp>

#include <render/renderer.hpp>

#include <ui/theme.hpp>
#include <ui/ui_tools.hpp>

class Window; // Forward declaration (we don't need the actual window class)
class PanelContainer;

struct PanelAdjacency {
    bool right, left, up, down = false;
};

class Panel {
protected:
    UILayout layout;
    Window* window;
    PanelContainer* container;
public:
    Panel(Window* window, PanelContainer* container) : window(window), container(container), layout(UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, 0, 1.0f, 0)))) {}

    virtual void update(float deltaTime, const UIBounds& bounds, PanelAdjacency adjacency) = 0;
    virtual void render(const UIBounds& bounds, PanelAdjacency adjacency) = 0;

    virtual bool processWindowInput(WindowInput& input, const UIBounds& bounds, PanelAdjacency adjacency) = 0;
    virtual void observeWindowInput(WindowInput& input, const UIBounds& bounds, PanelAdjacency adjacency) = 0;
};