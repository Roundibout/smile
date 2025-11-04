#pragma once

#include <string>
#include <vector>

#include <ui/widgets/widget.hpp>

#include <ui/widgets/tab/tab.hpp>

enum class TabBarDirection {
    Horizontal,
    Vertical
};

class TabBar : public Widget {
private:
    std::vector<Tab> tabs;
    size_t openedTab = 0;

    TabBarDirection direction;
public:
    TabBar(Window* window, UILayout layout, TabBarDirection direction) : Widget(window, layout), direction(direction) {}

    void update(float deltaTime, const UIBounds& bounds) override;
    void render(const UIBounds& bounds) override;
    bool processWindowInput(WindowInput& input, const UIBounds& bounds) override;
    void observeWindowInput(WindowInput& input, const UIBounds& bounds) override;

    Tab& createTab(std::string name);
};