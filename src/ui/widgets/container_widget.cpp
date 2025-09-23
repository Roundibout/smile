#include "container_widget.hpp"
#include <window/window.hpp>

void ContainerWidget::update(float deltaTime) {
    for (auto& child : children) {
        child->update(deltaTime);
    }
}

void ContainerWidget::render(UIBounds bounds) {
    Rect resolved = layout.resolve(bounds);
    window->renderer.drawRect(resolved.position, resolved.size, Color4(1.0f, 0.0f, 0.0f, 0.5f));

    UIBounds applied = bounds.applyLayout(layout);
    for (auto& child : children) {
        child->render(applied);
    }
}

void ContainerWidget::processWindowInput(WindowInput& input) {
    for (auto& child : children) {
        child->processWindowInput(input);
    }
}