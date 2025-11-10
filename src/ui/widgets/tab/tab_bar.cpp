#include "tab_bar.hpp"
#include <core/app.hpp>
#include <window/window.hpp>

void TabBar::update(float deltaTime, const UIBounds& bounds) {

}

void TabBar::render(const UIBounds& bounds) {
    UIBounds applied = window->renderer.applyLayout(bounds, layout);
    AbsoluteLayout resolved = window->renderer.resolveLayout(layout, bounds);

    UILayout tabLayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(0.0f, tabLength, 1.0f, 0)));
    tabLayout.cornerLT = UIDim(0.0f, 10);
    tabLayout.cornerRT = UIDim(0.0f, 10);

    int position = 0;
    for (auto& [tabId, tab] : tabs) {
        tabLayout.rect.position.x.offset = position + spacing / 2;
        window->renderer.drawRoundedStrokeRect(tabLayout, applied, app.theme.getColor(ThemeColor::ProjectTab), 2, app.theme.getColor(ThemeColor::ProjectTabStroke), UIStrokeAlignment::Outside);

        position += tabLength + spacing;
    }
}

bool TabBar::processWindowInput(WindowInput& input, const UIBounds& bounds) {
    return false;
}

void TabBar::observeWindowInput(WindowInput& input, const UIBounds& bounds) {
    
}

TabId TabBar::addTab(std::string name) {
    TabId tabId = ++nextId;
    tabs[tabId] = Tab{name};

    window->renderer.dirty();

    return tabId;
}