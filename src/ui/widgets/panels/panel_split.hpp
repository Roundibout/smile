#pragma once

#include <memory>
#include <algorithm>

#include <ui/widgets/panels/panel.hpp>
#include <ui/widgets/panels/panel_leaf.hpp>

enum class PanelSplitDirection {
    Horizontal,
    Vertical
};

enum class PanelSplitPlacement {
    First,
    Second
};

class PanelSplit : public Panel {
private:
    PanelSplitDirection direction;
    float ratio;
    std::unique_ptr<Panel> panel1; // object of abstract class type "Panel" is not allowed
    std::unique_ptr<Panel> panel2;

    bool resizing = false;

    void applyLayoutAndAdjacencies(const UIBounds& bounds, UIBounds& first, UIBounds& second, PanelAdjacency& firstAdj, PanelAdjacency& secondAdj);
    bool isOverResizeArea(const Vector2& point, const UIBounds& bounds);
    float getPositionOverArea(const Vector2& point, const UIBounds& bounds);
    void stopResizing();
public:
    PanelSplit(Window* window, PanelContainer* container, std::unique_ptr<Panel> existingPanel, PanelSplitDirection splitDirection, float splitRatio, PanelSplitPlacement existingPlacement);

    void update(float deltaTime, const UIBounds& bounds, PanelAdjacency adjacency) override;
    void render(const UIBounds& bounds, PanelAdjacency adjacency) override;

    bool processWindowInput(WindowInput& input, const UIBounds& bounds, PanelAdjacency adjacency) override;
    void observeWindowInput(WindowInput& input, const UIBounds& bounds, PanelAdjacency adjacency) override;

    Panel* getPanel(PanelSplitPlacement placement);
    PanelSplit* splitPanel(PanelSplitPlacement placement, PanelSplitDirection splitDirection, float splitRatio, PanelSplitPlacement existingPlacement);
};