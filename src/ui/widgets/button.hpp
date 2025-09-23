#pragma once

#include <ui/widgets/widget.hpp>

class Button : public Widget {
protected:
    bool hovered = false;
public:
    Button(Window* window, UILayout layout) : Widget(window, layout) {}

    void update(float deltaTime) {}
    void render(UIBounds bounds) {}
    void processWindowInput(WindowInput& input) override;
};