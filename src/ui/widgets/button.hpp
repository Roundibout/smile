#pragma once

#include <ui/widgets/widget.hpp>

class Button : public Widget {
protected:
    bool hovered = false;

    void deselect();
public:
    Button(Window* window, UILayout layout) : Widget(window, layout) {}

    void update(float deltaTime, const UIBounds& bounds) override {}
    void render(const UIBounds& bounds) override;
    bool processWindowInput(WindowInput& input, const UIBounds& bounds) override;
    void observeWindowInput(WindowInput& input, const UIBounds& bounds) override;
};