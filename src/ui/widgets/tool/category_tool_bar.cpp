#include "category_tool_bar.hpp"
#include <core/app.hpp>
#include <window/window.hpp>

void CategoryToolBar::render(const UIBounds& bounds) {
    UIBounds applied = window->renderer.applyLayout(bounds, layout);

    // Render non-hovered and non-selected tools first
    runForEach([this, applied](const UILayout& toolLayout, const CategoryToolBarEntry& tool, const ToolBarCategory& category) {
        if ((hovered && hoveredTool == tool.id) || selectedTool == tool.id) return;
        Color4 color = app.theme.getColor(ThemeColor::Button);
        Color4 stroke = app.theme.getColor(ThemeColor::ButtonStroke);
        window->renderer.drawRoundedStrokeRect(toolLayout, applied, color, 2, stroke, UIStrokeAlignment::Middle);
        window->renderer.drawText(toolLayout.rect.position + UIDim2(0.0f, toolSize + 10, 0.0f, toolSize / 2 - 10), applied, tool.name, app.theme.getFont(ThemeFont::Regular), 20, Color4());
    });

    // Render hovered tools to appear between
    runForEach([this, applied](const UILayout& toolLayout, const CategoryToolBarEntry& tool, const ToolBarCategory& category) {
        if (!hovered || hoveredTool != tool.id || selectedTool == tool.id) return;
        Color4 color = app.theme.getColor(ThemeColor::ButtonHovered);
        Color4 stroke = app.theme.getColor(ThemeColor::ButtonStrokeHovered);
        window->renderer.drawRoundedStrokeRect(toolLayout, applied, color, 2, stroke, UIStrokeAlignment::Middle);
        window->renderer.drawText(toolLayout.rect.position + UIDim2(0.0f, toolSize + 10, 0.0f, toolSize / 2 - 10), applied, tool.name, app.theme.getFont(ThemeFont::Regular), 20, Color4());
    });

    // Render selected tools to appear on top
    runForEach([this, applied](const UILayout& toolLayout, const CategoryToolBarEntry& tool, const ToolBarCategory& category) {
        if (selectedTool != tool.id) return;
        Color4 color = app.theme.getColor(ThemeColor::Accent);
        Color4 stroke = app.theme.getColor(ThemeColor::AccentHighlight);
        window->renderer.drawRoundedStrokeRect(toolLayout, applied, color, 2, stroke, UIStrokeAlignment::Outside);
        window->renderer.drawText(toolLayout.rect.position + UIDim2(0.0f, toolSize + 10, 0.0f, toolSize / 2 - 10), applied, tool.name, app.theme.getFont(ThemeFont::Regular), 20, Color4());
    });
};

bool CategoryToolBar::processWindowInput(WindowInput& input, const UIBounds& bounds) {
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

    runForEach([this, hoverable, down, up, &anyHovered, input, applied](const UILayout& toolLayout, const CategoryToolBarEntry& tool, const ToolBarCategory& category) {
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

void CategoryToolBar::observeWindowInput(WindowInput& input, const UIBounds& bounds) {

};

ToolEntryId CategoryToolBar::addTool(std::string name, std::string category) {
    ToolEntryId toolId = nextId++;
    
    auto it = std::find_if(categories.begin(), categories.end(), [&](const ToolBarCategory& c) {
        return c.name == category;
    });

    if (it == categories.end()) {
        categories.emplace_back(category);
        it = categories.end() - 1;
    }

    it->tools.emplace_back(toolId, name, category);

    window->renderer.dirty();

    return toolId;
}

bool CategoryToolBar::removeTool(ToolEntryId id) {
    for (auto catIt = categories.begin(); catIt != categories.end(); ++catIt) {
        auto& tools = catIt->tools;

        for (auto toolIt = tools.begin(); toolIt != tools.end(); ++toolIt) {
            if (toolIt->id == id) {
                tools.erase(toolIt);

                // If the category is now empty, remove it
                if (tools.empty()) {
                    categories.erase(catIt);
                }

                // Clean up state if needed
                if (selectedTool == id) selectedTool = 0;
                if (selectingTool == id) selecting = false;
                if (hoveredTool == id) hoveredTool = false;

                return true; // Found
            }
        }
    }
    return false; // Not found
}

void CategoryToolBar::clearTools() {
    categories.clear();
    nextId = 0;

    selectedTool = 0;
    selecting = false;
    selectingTool = 0;
    hovered = false;
    hoveredTool = 0;
}