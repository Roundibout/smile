#include "panel_leaf.hpp"
#include <core/app.hpp>
#include <window/window.hpp>
#include <ui/widgets/panels/panel_container.hpp>

UILayout PanelLeaf::applyInsetToLayout(const UILayout& childLayout, const PanelAdjacency& adjacency) {
    UILayout insetLayout = childLayout;

    int margin = app.theme.getMetricInt(ThemeMetric::PanelMargin);
    int stroke = app.theme.getMetricInt(ThemeMetric::PanelStroke);

    int posX = margin;
    int posY = margin;

    int sizeX = margin * 2;
    int sizeY = margin * 2;

    if (adjacency.right) {
        sizeX -= margin / 2 - stroke / 2;
    }
    if (adjacency.left) {
        sizeX -= margin / 2 - stroke / 2;
        posX -= margin / 2 - stroke / 2;
    }
    if (adjacency.up) {
        sizeY -= margin / 2 - stroke / 2;
    }
    if (adjacency.down) {
        sizeY -= margin / 2 - stroke / 2;
        posY -= margin / 2 - stroke / 2;
    }

    insetLayout.rect.position += UIDim2(0.0f, posX, 0.0f, posY);
    insetLayout.rect.size -= UIDim2(0.0f, sizeX, 0.0f, sizeY);

    return insetLayout;
}

void PanelLeaf::update(float deltaTime, const UIBounds& bounds, PanelAdjacency adjacency) {
    UIBounds applied = window->renderer.applyLayout(bounds, applyInsetToLayout(layout, adjacency));
    
    for (auto& child : children) {
        child->update(deltaTime, applied);
    }
}

void PanelLeaf::render(const UIBounds& bounds, PanelAdjacency adjacency) {
    UILayout inset = applyInsetToLayout(layout, adjacency);
    inset.setCorners(UIDim(0.0f, app.theme.getMetric(ThemeMetric::PanelCorner)));

    window->renderer.drawRoundedStrokeRect(inset, bounds, app.theme.getColor(ThemeColor::Panel), app.theme.getMetricInt(ThemeMetric::PanelStroke), app.theme.getColor(ThemeColor::PanelStroke));

    UIBounds applied = window->renderer.applyLayout(bounds, applyInsetToLayout(layout, adjacency));

    for (auto& child : children) {
        child->render(applied);
    }
}

bool PanelLeaf::processWindowInput(WindowInput& input, const UIBounds& bounds, PanelAdjacency adjacency) {
    UIBounds applied = window->renderer.applyLayout(bounds, applyInsetToLayout(layout, adjacency));
    
    bool consumed = false;
    for (auto it = children.rbegin(); it != children.rend(); ++it) {
        if (consumed == false) {
            consumed = (*it)->processWindowInput(input, applied);
        } else {
            (*it)->observeWindowInput(input, applied);
        }
    }

    return consumed;
}

void PanelLeaf::observeWindowInput(WindowInput& input, const UIBounds& bounds, PanelAdjacency adjacency) {
    UIBounds applied = window->renderer.applyLayout(bounds, applyInsetToLayout(layout, adjacency));

    for (auto& child : children) {
        child->observeWindowInput(input, applied);
    }
}