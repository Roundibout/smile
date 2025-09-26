#include "button.hpp"
#include <window/window.hpp>

void Button::render(UIBounds bounds) {
    Rect resolved = layout.resolve(bounds);
    std::cout << resolved.size.to_string() << ", " << resolved.position.to_string() << std::endl;
    window->renderer.drawRoundedRect(resolved.position - Vector2(2.0f, 2.0f), resolved.size + Vector2(4.0f, 4.0f), Color4(0.2f, 0.2f, 0.25f, 1.0f), UIDim(1.0f, 2));
    window->renderer.drawRoundedRect(resolved.position, resolved.size, Color4(0.08f, 0.08f, 0.11f, 1.0f), UIDim(1.0f, 0));
}

void Button::processWindowInput(WindowInput& input) {
    if (input.type == WindowInputType::MouseMove) {
        
    }
}