#include "container_widget.hpp"
#include <window/window.hpp>

void ContainerWidget::update(float deltaTime, const UIBounds& bounds) {
    UIBounds applied = window->renderer.applyLayout(bounds, layout);
    
    for (auto& child : children) {
        child->update(deltaTime, applied);
    }
}

void ContainerWidget::render(const UIBounds& bounds) {
    UIBounds applied = window->renderer.applyLayout(bounds, layout);

    for (auto& child : children) {
        child->render(applied);
    }
}

bool ContainerWidget::processWindowInput(WindowInput& input, const UIBounds& bounds) {
    UIBounds applied = window->renderer.applyLayout(bounds, layout);
    
    bool consumed = false;
    for (auto it = children.rbegin(); it != children.rend(); ++it) {
        if (consumed == false) {
            consumed = (*it)->processWindowInput(input, applied);
        } else {
            (*it)->observeWindowInput(input, applied);
        }
    }

    return consumed;
}

void ContainerWidget::observeWindowInput(WindowInput& input, const UIBounds& bounds) {
    UIBounds applied = window->renderer.applyLayout(bounds, layout);

    for (auto& child : children) {
        child->observeWindowInput(input, applied);
    }
}