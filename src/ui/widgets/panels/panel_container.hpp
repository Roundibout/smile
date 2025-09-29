#pragma once

#include <memory>

#include <ui/widgets/widget.hpp>
#include <ui/widgets/panels/panel.hpp>
#include <ui/widgets/panels/panel_leaf.hpp>
#include <ui/widgets/panels/panel_split.hpp>

class PanelContainer : public Widget {
private:
    std::unique_ptr<Panel> panel;
    bool resizeHovered = false;
    PanelSplitDirection hoveredDirection;
    bool resizing = false;
public:
    PanelContainer(Window* window, UILayout layout);

    void update(float deltaTime, const UIBounds& bounds) override;
    void render(const UIBounds& bounds) override;

    bool processWindowInput(WindowInput& input, const UIBounds& bounds) override;
    void observeWindowInput(WindowInput& input, const UIBounds& bounds) override;

    Panel* getPanel();
    PanelSplit* splitPanel(PanelSplitDirection splitDirection, float splitRatio, PanelSplitPlacement existingPlacement);

    void setResizeHovered(PanelSplitDirection direction);
    void setResize();
    void releaseResize();
};