#include "tab_bar.hpp"
#include <window/window.hpp>

void TabBar::update(float deltaTime, const UIBounds& bounds) {

}

void TabBar::render(const UIBounds& bounds) {
    
}

bool TabBar::processWindowInput(WindowInput& input, const UIBounds& bounds) {
    return false;
}

void TabBar::observeWindowInput(WindowInput& input, const UIBounds& bounds) {
    
}

Tab& TabBar::createTab(std::string name) {
    tabs.emplace_back(window, UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, 0, 1.0f, 0))), name);
    return tabs.back();
}