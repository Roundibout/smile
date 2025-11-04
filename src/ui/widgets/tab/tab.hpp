#pragma once

#include <string>

#include <ui/widgets/widget.hpp>

class Tab : public Widget {
private:
    bool hovered = false;

    std::string name;

    void deselect();
public:
    Tab(Window* window, UILayout layout, std::string name) : Widget(window, layout), name(name) {}

    void update(float deltaTime, const UIBounds& bounds) override;
    void render(const UIBounds& bounds) override;
    bool processWindowInput(WindowInput& input, const UIBounds& bounds) override;
    void observeWindowInput(WindowInput& input, const UIBounds& bounds) override;
};