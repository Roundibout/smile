#include "panel_split.hpp"
#include <window/window.hpp>
#include <ui/widgets/panels/panel_container.hpp>

PanelSplit::PanelSplit(Window* window, PanelContainer* container, std::unique_ptr<Panel> existingPanel, PanelSplitDirection splitDirection, float splitRatio, PanelSplitPlacement existingPlacement) : Panel(window, container), direction(splitDirection), ratio(std::clamp(splitRatio, 0.05f, 0.95f)) {
    if (existingPlacement == PanelSplitPlacement::First) {
        panel1 = std::move(existingPanel);
        panel2 = std::make_unique<PanelLeaf>(window, container);
    } else {
        panel1 = std::make_unique<PanelLeaf>(window, container);
        panel2 = std::move(existingPanel);
    }
}

void PanelSplit::applyLayoutAndAdjacencies(const UIBounds& bounds, UIBounds& first, UIBounds& second, PanelAdjacency& firstAdj, PanelAdjacency& secondAdj) {
    if (direction == PanelSplitDirection::Vertical) {
        first = window->renderer.applyLayout(bounds, UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(ratio, 0, 1.0f, 0))));
        second = window->renderer.applyLayout(bounds, UILayout(UIRect(UIDim2(ratio, 0, 0.0f, 0), UIDim2(1.0f - ratio, 0, 1.0f, 0))));

        firstAdj.right = true;
        secondAdj.left = true;
    } else {
        first = window->renderer.applyLayout(bounds, UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, 0, ratio, 0))));
        second = window->renderer.applyLayout(bounds, UILayout(UIRect(UIDim2(0.0f, 0, ratio, 0), UIDim2(1.0f, 0, 1.0f - ratio, 0))));

        firstAdj.up = true;
        secondAdj.down = true;
    }
}

bool PanelSplit::isOverResizeArea(const Vector2& point, const UIBounds& bounds) {
    int margin = Theme::metricInt(ThemeMetric::PanelMargin);

    UILayout selectionArea;
    if (direction == PanelSplitDirection::Vertical) {
        selectionArea = UILayout(UIRect(UIDim2(ratio, -margin / 2, 0.0f, 0), UIDim2(0.0f, margin, 1.0f, 0)));
    } else {
        selectionArea = UILayout(UIRect(UIDim2(0.0f, 0, ratio, -margin / 2), UIDim2(1.0f, 0, 0.0f, margin)));
    }

    if (UITools::isPointOverRect(point, window->renderer.resolveLayout(selectionArea, bounds))) {
        return true;
    }
    return false;
}

float PanelSplit::getPositionOverArea(const Vector2& point, const UIBounds& bounds) {
    Rect area = window->renderer.resolveLayout(layout, bounds).rect;

    if (direction == PanelSplitDirection::Vertical) {
        return std::clamp(static_cast<float>(point.x - area.position.x) / static_cast<float>(area.size.x), 0.05f, 0.95f);
    } else {
        return std::clamp(static_cast<float>(point.y - area.position.y) / static_cast<float>(area.size.y), 0.05f, 0.95f);
    }
}

void PanelSplit::stopResizing() {
    if (resizing == true) {
        resizing = false;
        container->releaseResize();
        window->releaseCapture();
        window->setCursor(Cursor::Arrow);
    }
}

void PanelSplit::update(float deltaTime, const UIBounds& bounds, PanelAdjacency adjacency) {
    UIBounds first;
    UIBounds second;
    PanelAdjacency firstAdj = adjacency;
    PanelAdjacency secondAdj = adjacency;

    applyLayoutAndAdjacencies(bounds, first, second, firstAdj, secondAdj);

    panel1->update(deltaTime, first, firstAdj);
    panel2->update(deltaTime, second, secondAdj);
}

void PanelSplit::render(const UIBounds& bounds, PanelAdjacency adjacency) {
    UIBounds first;
    UIBounds second;
    PanelAdjacency firstAdj = adjacency;
    PanelAdjacency secondAdj = adjacency;

    applyLayoutAndAdjacencies(bounds, first, second, firstAdj, secondAdj);

    panel1->render(first, firstAdj);
    panel2->render(second, secondAdj);
}

bool PanelSplit::processWindowInput(WindowInput& input, const UIBounds& bounds, PanelAdjacency adjacency) {
    UIBounds first;
    UIBounds second;
    PanelAdjacency firstAdj = adjacency;
    PanelAdjacency secondAdj = adjacency;

    applyLayoutAndAdjacencies(bounds, first, second, firstAdj, secondAdj);

    bool consumed = false;
    if (input.type == WindowInputType::MouseButtonDown) {
        if (input.mouse.button == MouseButton::Left) {
            if (isOverResizeArea(input.mouse.position, bounds)) {
                resizing = true;

                container->setResize();

                window->setCapture();
                window->setCursor(direction == PanelSplitDirection::Vertical ? Cursor::HorizontalResize : Cursor::VerticalResize);

                ratio = getPositionOverArea(input.mouse.position, bounds);
                window->renderer.dirty();

                consumed = true;
            }
        }
    } else if (input.type == WindowInputType::MouseButtonUp) {
        if (input.mouse.button == MouseButton::Left) {
            stopResizing();
        }
    } else if (input.type == WindowInputType::MouseMove) {
        if (input.mouse.button == MouseButton::Left) {

            if (isOverResizeArea(input.mouse.position, bounds)) {
                container->setResizeHovered(direction);
            }

            if (resizing == true) {
                ratio = getPositionOverArea(input.mouse.position, bounds);
                window->renderer.dirty();

                consumed = true;
            }
        }
    }

    if (consumed == false) {
        consumed = panel1->processWindowInput(input, first, firstAdj);
        if (!consumed) {
            if (!panel2->processWindowInput(input, second, secondAdj)) {
                return false;
            }
        } else {
            panel2->observeWindowInput(input, second, secondAdj);
        }
    } else {
        panel1->observeWindowInput(input, first, firstAdj);
        panel2->observeWindowInput(input, second, secondAdj);
    }
    return true;
}

void PanelSplit::observeWindowInput(WindowInput& input, const UIBounds& bounds, PanelAdjacency adjacency) {
    UIBounds first;
    UIBounds second;
    PanelAdjacency firstAdj = adjacency;
    PanelAdjacency secondAdj = adjacency;

    applyLayoutAndAdjacencies(bounds, first, second, firstAdj, secondAdj);

    if (input.type == WindowInputType::MouseButtonUp) {
        if (input.mouse.button == MouseButton::Left) {
            stopResizing();
        }
    }

    panel1->observeWindowInput(input, first, firstAdj);
    panel2->observeWindowInput(input, second, secondAdj);
}

Panel* PanelSplit::getPanel(PanelSplitPlacement placement) {
    if (placement == PanelSplitPlacement::First) {
        return panel1.get();
    } else {
        return panel2.get();
    }
}

PanelSplit* PanelSplit::splitPanel(PanelSplitPlacement placement, PanelSplitDirection splitDirection, float splitRatio, PanelSplitPlacement existingPlacement) {
    if (placement == PanelSplitPlacement::First) {
        std::unique_ptr<PanelSplit> split = std::make_unique<PanelSplit>(window, container, std::move(panel1), splitDirection, splitRatio, existingPlacement);
        PanelSplit* rawPtr = split.get();
        panel1 = std::move(split);
        return rawPtr;
    } else {
        std::unique_ptr<PanelSplit> split = std::make_unique<PanelSplit>(window, container, std::move(panel2), splitDirection, splitRatio, existingPlacement);
        PanelSplit* rawPtr = split.get();
        panel2 = std::move(split);
        return rawPtr;
    }
}