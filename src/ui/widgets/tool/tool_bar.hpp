#pragma once

#include <string>
#include <vector>
#include <vector>
#include <cstdint>

#include <ui/widgets/widget.hpp>

using ToolEntryId = uint32_t;

struct ToolBarEntry {
    ToolEntryId id;
    std::string name;

    ToolBarEntry(ToolEntryId id, const std::string& name) : id(id), name(name) {}
};

class ToolBar : public Widget {
private:
    std::vector<ToolBarEntry> tools;
    ToolEntryId nextId = 0;

    int toolSize = 64;
    
    ToolEntryId selectedTool = 0; // The tool that is actually selected
    bool selecting = false;
    ToolEntryId selectingTool = 0; // A tool that we have clicked down on but haven't released on it

    bool hovered = false;
    ToolEntryId hoveredTool = 0;

    template<typename Function>
    void runForEach(Function func) {
        UILayout toolLayout(UIRect(UIDim2(0.0f, 0, 1.0f, -toolSize), UIDim2(0.0f, toolSize, 0.0f, toolSize)));

        int position = toolSize;
        for (auto it = tools.begin(); it != tools.end(); ++it) {
            bool first = (it == tools.begin());
            bool last = (it == std::prev(tools.end()));

            ToolBarEntry& tool = *it;

            toolLayout.rect.position.y.offset = -position;

            toolLayout.setCorners(UIDim(0.0f, 0));
            if (first) {
                toolLayout.cornerLT = UIDim(0.0f, 10);
                toolLayout.cornerRT = UIDim(0.0f, 10);
            } else if (last) {
                toolLayout.cornerLB = UIDim(0.0f, 10);
                toolLayout.cornerRB = UIDim(0.0f, 10);
            }

            func(toolLayout, tool);

            position += toolSize;
        }
    }
public:
    ToolBar(Window* window, UILayout layout) : Widget(window, UILayout(layout.rect.position, UIDim2(UIDim(0.0f, 60), layout.rect.size.y))) {} // TODO: add changeable tool size? replace 60 with tool size

    void update(float deltaTime, const UIBounds& bounds) {}
    void render(const UIBounds& bounds) override;
    bool processWindowInput(WindowInput& input, const UIBounds& bounds) override;
    void observeWindowInput(WindowInput& input, const UIBounds& bounds) override;

    ToolEntryId addTool(std::string name);
    void clearTools();
};