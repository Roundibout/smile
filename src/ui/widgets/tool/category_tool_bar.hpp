#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

#include "type/numbers.hpp"

#include <core/signal.hpp>

#include <ui/widgets/widget.hpp>

#include <render/font_manager.hpp>

using ToolEntryId = u32;

struct CategoryToolBarEntry {
    ToolEntryId id;
    std::string name;
    std::string category;

    CategoryToolBarEntry(
        ToolEntryId id, 
        const std::string& name, 
        const std::string& category
    ) : id(id), name(name), category(category) {}
};

struct ToolBarCategory {
    std::string name;
    std::vector<CategoryToolBarEntry> tools;

    ToolBarCategory(const std::string& name) : name(name) {}
};

class CategoryToolBar : public Widget {
private:
    std::vector<ToolBarCategory> categories;
    ToolEntryId nextId = 0;

    int toolSize = 64;
    int spacing = 16;

    ToolEntryId selectedTool = 0; // The tool that is actually selected
    bool selecting = false;
    ToolEntryId selectingTool = 0; // A tool that we have clicked down on but haven't released on it

    bool hovered = false;
    ToolEntryId hoveredTool = 0;

    template<typename Function>
    void runForEach(Function func) {
        UILayout toolLayout(UIRect(UIDim2(0.0f, 0, 1.0f, -toolSize), UIDim2(0.0f, toolSize, 0.0f, toolSize)));

        int position = toolSize;

        for (auto itC = categories.begin(); itC != categories.end(); ++itC) {
            ToolBarCategory& category = *itC;

            for (auto it = category.tools.begin(); it != category.tools.end(); ++it) {
                bool first = (it == category.tools.begin());
                bool last = (it == std::prev(category.tools.end()));

                CategoryToolBarEntry& tool = *it;

                toolLayout.rect.position.y.offset = -position;

                toolLayout.setCorners(UIDim(0.0f, 0));
                if (first) {
                    toolLayout.cornerLT = UIDim(0.0f, 6);
                    toolLayout.cornerRT = UIDim(0.0f, 6);
                }
                if (last) {
                    toolLayout.cornerLB = UIDim(0.0f, 6);
                    toolLayout.cornerRB = UIDim(0.0f, 6);
                }

                func(toolLayout, tool, category);

                position += toolSize;
            }

            position += spacing;
        }
    }
public:
    Signal<ToolEntryId> onToolSelected;

    CategoryToolBar(
        App& app, 
        Window* window, 
        UILayout layout
    ) :
        Widget(
            app, 
            window, 
            UILayout(
                layout.rect.position, 
                UIDim2(UIDim(0.0f, 60), layout.rect.size.y) // TODO: add changeable tool size? replace 60 with tool size
            )
        ) 
    {}

    void update(float deltaTime, const UIBounds& bounds) override {}
    void render(const UIBounds& bounds) override;
    bool processWindowInput(WindowInput& input, const UIBounds& bounds) override;
    void observeWindowInput(WindowInput& input, const UIBounds& bounds) override;

    ToolEntryId addTool(std::string name, std::string category);
    // Doesn't fire onToolSelected signal
    bool selectTool(ToolEntryId id);
    bool removeTool(ToolEntryId id);
    void clearTools();
};