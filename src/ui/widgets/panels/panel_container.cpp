#include "panel_container.hpp"
#include <window/window.hpp>

PanelContainer::PanelContainer(Window* window, UILayout layout) : 
    Widget(window, layout), 
    panel(std::make_unique<PanelLeaf>(window, this)) {}

void PanelContainer::update(float deltaTime, const UIBounds& bounds) {
    UIBounds applied = window->renderer.applyLayout(bounds, layout);
    panel->update(deltaTime, applied, PanelAdjacency());
}

void PanelContainer::render(const UIBounds& bounds) {
    UIBounds applied = window->renderer.applyLayout(bounds, layout);
    panel->render(applied, PanelAdjacency());
}

bool PanelContainer::processWindowInput(WindowInput& input, const UIBounds& bounds) {
    UIBounds applied = window->renderer.applyLayout(bounds, layout);

    std::cout << resizeHovered << "\n";

    if (resizing == false) {
        if (input.type == WindowInputType::MouseMove) {
            resizeHovered = false;
        }

        bool consumed = panel->processWindowInput(input, applied, PanelAdjacency());

        if (input.type == WindowInputType::MouseMove) {
            if (resizeHovered == true) {
                window->setCursor(hoveredDirection == PanelSplitDirection::Vertical ? Cursor::HorizontalResize : Cursor::VerticalResize);
            } else {
                window->setCursor(Cursor::Arrow);
            }
        }
        
        return consumed;
    } else {
        return panel->processWindowInput(input, applied, PanelAdjacency());
    }
}

void PanelContainer::observeWindowInput(WindowInput& input, const UIBounds& bounds) {
    UIBounds applied = window->renderer.applyLayout(bounds, layout);
    panel->observeWindowInput(input, applied, PanelAdjacency());
}

Panel* PanelContainer::getPanel() {
    return panel.get();
}

PanelSplit* PanelContainer::splitPanel(PanelSplitDirection splitDirection, float splitRatio, PanelSplitPlacement existingPlacement) {
    std::unique_ptr<PanelSplit> split = std::make_unique<PanelSplit>(window, this, std::move(panel), splitDirection, splitRatio, existingPlacement);
    PanelSplit* rawPtr = split.get();
    panel = std::move(split);
    return rawPtr;
}

void PanelContainer::setResizeHovered(PanelSplitDirection direction) {
    resizeHovered = true;
    hoveredDirection = direction;
}

void PanelContainer::setResize() {
    resizing = true;
}

void PanelContainer::releaseResize() {
    resizing = false;
}