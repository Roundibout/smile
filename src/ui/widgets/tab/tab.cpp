#include "tab.hpp"
#include <window/window.hpp>

void Tab::update(float deltaTime, const UIBounds& bounds) {

}

void Tab::render(const UIBounds& bounds) {
    layout.cornerLT = UIDim(0.0f, 10);
    layout.cornerRT = UIDim(0.0f, 10);
    window->renderer.drawRoundedStrokeRect(layout, bounds, Theme::color(ThemeColor::ProjectTab), 2, Theme::color(ThemeColor::ProjectTabStroke), UIStrokeAlignment::Outside);
}

bool Tab::processWindowInput(WindowInput& input, const UIBounds& bounds) {
    return false;
}

void Tab::observeWindowInput(WindowInput& input, const UIBounds& bounds) {
    
}