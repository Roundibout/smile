#include "button.hpp"
#include <core/app.hpp>
#include <window/window.hpp>

void Button::deselect() {
    if (hovered == true) window->renderer.dirty();
    hovered = false;
}

void Button::render(const UIBounds& bounds) {
    if (hovered == true) {
        window->renderer.drawRoundedStrokeRect(layout, bounds, Color4(0.18f, 0.18f, 0.21f, 1.0f), 1, Color4(0.3f, 0.3f, 0.35f, 1.0f));
        window->renderer.drawText(layout.rect.position + UIDim2(0.0f, 10, 0.0f, 10), bounds, "Test", "assets/fonts/Inter_18pt-Regular.ttf", 40, Color4(0.9f, 0.9f, 0.9f, 1.0f));
    } else {
        window->renderer.drawRoundedStrokeRect(layout, bounds, Color4(0.08f, 0.08f, 0.11f, 1.0f), 1, Color4(0.2f, 0.2f, 0.25f, 1.0f));
        window->renderer.drawText(layout.rect.position + UIDim2(0.0f, 10, 0.0f, 10), bounds, "Test", "assets/fonts/Inter_18pt-Regular.ttf", 40, Color4(0.8f, 0.8f, 0.8f, 1.0f));
    }
}

bool Button::processWindowInput(WindowInput& input, const UIBounds& bounds) {
    if (input.type == WindowInputType::MouseMove) {
        AbsoluteLayout resolved = window->renderer.resolveLayout(layout, bounds);
        if (UITools::isPointOverRoundedRect(input.mouse.position, resolved)) {
            if (hovered == false) window->renderer.dirty();
            hovered = true;
            return true;
        } else {
            deselect();
        }
    }
    return false;
}

void Button::observeWindowInput(WindowInput& input, const UIBounds& bounds) {
    if (input.type == WindowInputType::MouseMove) {
        deselect();
    }
}