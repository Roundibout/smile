#include "tool_bar.hpp"
#include <core/app.hpp>
#include <window/window.hpp>

void ToolBar::render(const UIBounds& bounds) {
    UIBounds applied = window->renderer.applyLayout(bounds, layout);

    // Render non-hovered and non-selected tools first
    runForEach([this, applied](const UILayout& toolLayout, const ToolBarEntry& tool) {
        if ((hovered && hoveredTool == tool.id) || selectedTool == tool.id) return;
        Color4 color = app.theme.getColor(ThemeColor::Button);
        Color4 stroke = app.theme.getColor(ThemeColor::ButtonStroke);
        window->renderer.drawRoundedStrokeRect(toolLayout, applied, color, 2, stroke, UIStrokeAlignment::Middle);
        window->renderer.drawText(toolLayout.rect.position + UIDim2(0.0f, toolSize + 10, 0.0f, toolSize / 2 - 10), applied, tool.name, app.theme.getFont(ThemeFont::Regular), 20, Color4());
    });

    // Render hovered tools to appear between
    runForEach([this, applied](const UILayout& toolLayout, const ToolBarEntry& tool) {
        if (!hovered || hoveredTool != tool.id || selectedTool == tool.id) return;
        Color4 color = app.theme.getColor(ThemeColor::ButtonHovered);
        Color4 stroke = app.theme.getColor(ThemeColor::ButtonStrokeHovered);
        window->renderer.drawRoundedStrokeRect(toolLayout, applied, color, 2, stroke, UIStrokeAlignment::Middle);
        window->renderer.drawText(toolLayout.rect.position + UIDim2(0.0f, toolSize + 10, 0.0f, toolSize / 2 - 10), applied, tool.name, app.theme.getFont(ThemeFont::Regular), 20, Color4());
    });

    // Render selected tools to appear on top
    runForEach([this, applied](const UILayout& toolLayout, const ToolBarEntry& tool) {
        if (selectedTool != tool.id) return;
        Color4 color = app.theme.getColor(ThemeColor::Accent);
        Color4 stroke = app.theme.getColor(ThemeColor::AccentHighlight);
        window->renderer.drawRoundedStrokeRect(toolLayout, applied, color, 2, stroke, UIStrokeAlignment::Middle);
        window->renderer.drawText(toolLayout.rect.position + UIDim2(0.0f, toolSize + 10, 0.0f, toolSize / 2 - 10), applied, tool.name, app.theme.getFont(ThemeFont::Regular), 20, Color4());
    });
};

bool ToolBar::processWindowInput(WindowInput& input, const UIBounds& bounds) {
    AbsoluteLayout resolvedArea = window->renderer.resolveLayout(layout, bounds);

    bool hoverable = false;
    bool down = false;
    bool up = false;

    // Check if the mouse is within the toolbar area
    if (UITools::isPointOverRect(input.mouse.position, resolvedArea)) {
        hoverable = true;
        if (input.type == WindowInputType::MouseButtonDown && input.mouse.button == MouseButton::Left) {
            down = true;
            selecting = false;
        } else if (input.type == WindowInputType::MouseButtonUp && input.mouse.button == MouseButton::Left) {
            up = true;
        }
    } else {
        if (hovered) {
            hovered = false;
            window->renderer.dirty();
        }
        return false;
    }

    UIBounds applied = window->renderer.applyLayout(bounds, layout);
    bool anyHovered = false;

    runForEach([this, hoverable, down, up, &anyHovered, input, applied](const UILayout& toolLayout, const ToolBarEntry& tool) {
        if (UITools::isPointOverRoundedRect(input.mouse.position, window->renderer.resolveLayout(toolLayout, applied))) {
            if (hoverable) {
                anyHovered = true;
                if (hoveredTool != tool.id) {
                    hoveredTool = tool.id;
                    window->renderer.dirty();
                }
                if (!hovered) {
                    hovered = true;
                    window->renderer.dirty();
                }
            }
            if (down) {
                selectingTool = tool.id;
                selecting = true;
            } else if (up) {
                if (selectingTool == tool.id && selectedTool != tool.id) {
                    selectedTool = tool.id;
                    onToolSelected.emit(selectedTool);
                    window->renderer.dirty();
                }
            }
        }
    });

    if (!anyHovered) {
        hovered = false;
        window->renderer.dirty();
    }

    return true;
};

void ToolBar::observeWindowInput(WindowInput& input, const UIBounds& bounds) {

};

ToolEntryId ToolBar::addTool(std::string name) {
    ToolEntryId toolId = nextId++;
    tools.emplace_back(toolId, name);

    window->renderer.dirty();

    return toolId;
}

bool ToolBar::selectTool(ToolEntryId id) {
    for (auto it = tools.begin(); it != tools.end(); ++it) {
        if (it->id == id) {
            selectedTool = it->id;

            window->renderer.dirty();

            return true; // Found
        }
    }
    return false; // Not found
}

bool ToolBar::removeTool(ToolEntryId id) {
    for (auto it = tools.begin(); it != tools.end(); ++it) {
        if (it->id == id) {
            tools.erase(it);

            // Clean up state if needed
            if (selectedTool == id) selectedTool = 0;
            if (selectingTool == id) selecting = false;
            if (hoveredTool == id) hoveredTool = false;

            window->renderer.dirty();

            return true; // Found
        }
    }
    return false; // Not found
}

void ToolBar::clearTools() {
    tools.clear();
    nextId = 0;

    selectedTool = 0;
    selecting = false;
    selectingTool = 0;
    hovered = false;
    hoveredTool = 0;

    window->renderer.dirty();
}