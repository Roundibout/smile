#include "tab_bar.hpp"
#include <window/window.hpp>

void TabBar::update(float deltaTime, const UIBounds& bounds) {

}

void TabBar::render(const UIBounds& bounds) {
    AbsoluteLayout applied = window->renderer.resolveLayout(layout, bounds);
    UIBounds areaBounds = window->renderer.applyLayout(bounds, layout);
    int position = 0;
    for (Tab tab : tabs) {
        tab.render(window->renderer.applyLayout(areaBounds, UILayout(UIRect(UIDim2(0.0f, position, 0.0f, 0), UIDim2(0.0f, tabLength + spacing, 1.0f, 0)))));
        position += tabLength + spacing;
    }
}

bool TabBar::processWindowInput(WindowInput& input, const UIBounds& bounds) {
    return false;
}

void TabBar::observeWindowInput(WindowInput& input, const UIBounds& bounds) {
    
}

Tab* TabBar::createTab(std::string name) {
    window->renderer.dirty();
    tabs.emplace_back(window, UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, -spacing, 1.0f, 0))), name);
    return &tabs.back();
}