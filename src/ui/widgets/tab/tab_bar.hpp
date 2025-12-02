#pragma once

#include <string>
#include <unordered_map>

#include "type/numbers.hpp"

#include <ui/widgets/widget.hpp>

using TabId = u32;

enum class TabBarDirection {
    Horizontal,
    Vertical
};

struct Tab {
    std::string name;
};

class TabBar : public Widget {
private:
    std::unordered_map<TabId, Tab> tabs;
    TabId nextId = 0;

    TabId openedId = 0;
    int tabLength = 200;
    int spacing = 10;

    TabBarDirection direction;
public:
    TabBar(
        App& app, 
        Window* window, 
        UILayout layout, 
        TabBarDirection direction
    ) :
        Widget(app, window, layout), 
        direction(direction) 
    {}

    void update(float deltaTime, const UIBounds& bounds) override;
    void render(const UIBounds& bounds) override;
    bool processWindowInput(WindowInput& input, const UIBounds& bounds) override;
    void observeWindowInput(WindowInput& input, const UIBounds& bounds) override;

    TabId addTab(std::string name);
};