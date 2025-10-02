#include "viewport.hpp"
#include <window/window.hpp>

void Viewport::render(const UIBounds& bounds) {
    layout.setCorners(UIDim(0.0f, Theme::get().getMetricInt(ThemeMetric::PanelCorner)));
    window->renderer.drawRoundedRect(layout, bounds, Color4(0.0f, 0.0f, 0.0f));
    window->renderer.drawText(UIDim2(0.0f, 10, 1.0f, -30), bounds, "Viewport", "assets/fonts/Inter_18pt-Regular.ttf", 20, Color4(0.5f, 0.5f, 0.5f));

    for (const Point& point : obj.points) {
        UIDim2 position = UIDim2(0.25f + point.x / 2, 0, 0.25f + point.y / 2, 0);
        window->renderer.drawRoundedRect(UILayout(UIRect(position, UIDim2(0.0f, 10, 0.0f, 10))), bounds, Color4());
        window->renderer.drawText(position + UIDim2(0.0f, 10, 0.0f, 10), bounds, std::to_string(point.id), "assets/fonts/Inter_18pt-Regular.ttf", 20, Color4());
    }
}