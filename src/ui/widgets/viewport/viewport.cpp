#include "viewport.hpp"
#include <window/window.hpp>

void Viewport::update(float deltaTime, const UIBounds& bounds) {
    timer += deltaTime;
    if (timer >= 1.0f) {
        timer -= 1.0f;
        if (computedRender) {
            computedRender = false;
        } else {
            computedRender = true;
        }
        window->renderer.dirty();
    }
}

void Viewport::render(const UIBounds& bounds) {
    layout.setCorners(UIDim(0.0f, Theme::metricInt(ThemeMetric::PanelCorner)));
    window->renderer.drawRoundedRect(layout, bounds, Color4(0.0f, 0.0f, 0.0f));

    if (!computedRender) {
        window->renderer.drawText(UIDim2(0.0f, 10, 1.0f, -30), bounds, "Viewport", Theme::font(ThemeFont::Regular), 20, Color4(1.0f, 1.0f, 1.0f, 0.5f));

        for (const Line& line : obj.lines) {
            const Point* point1 = obj.getPoint(line.point1);
            const Point* point2 = obj.getPoint(line.point2);
            UIDim2 position1 = UIDim2(0.25f + point1->x / 2, 0, 0.25f + point1->y / 2, 0);
            UIDim2 position2 = UIDim2(0.25f + point2->x / 2, 0, 0.25f + point2->y / 2, 0);

            float dx = point2->x - point1->x;
            float dy = point2->y - point1->y;
            float len = std::sqrt(dx*dx + dy*dy);
            if (len == 0) continue; // Skip degenerate line
            dx /= len;
            dy /= len;
            float px = -dy;
            float py = dx;
            px *= 1.0f;
            py *= 1.0f;

            UIDim2 offset1(0.0f, px + 5, 0.0f, py + 5);
            UIDim2 offset2(0.0f, -px + 5, 0.0f, -py + 5);

            window->renderer.drawQuad(
                position1 + offset1,
                position1 + offset2,
                position2 + offset2,
                position2 + offset1,
                bounds, Color4()
            );
        }

        for (const Point& point : obj.points) {
            UIDim2 position = UIDim2(0.25f + point.x / 2, 0, 0.25f + point.y / 2, 0);
            window->renderer.drawRoundedRect(UILayout(UIRect(position, UIDim2(0.0f, 10, 0.0f, 10))), bounds, Color4());
            window->renderer.drawText(position + UIDim2(0.0f, 10, 0.0f, 10), bounds, std::to_string(point.id), Theme::font(ThemeFont::Bold), 20, Color4());
        }
    } else {
        window->renderer.drawText(UIDim2(0.0f, 10, 1.0f, -30), bounds, "Viewport (Computed)", Theme::font(ThemeFont::Regular), 20, Color4(1.0f, 1.0f, 1.0f, 0.5f));

        for (const Edge& edge : obj.edges) {
            const Vertex& vertex1 = obj.vertices[obj.vertexIdToIndex[edge.vertex1]];
            const Vertex& vertex2 = obj.vertices[obj.vertexIdToIndex[edge.vertex2]];
            UIDim2 position1 = UIDim2(0.25f + vertex1.x / 2, 0, 0.25f + vertex1.y / 2, 0);
            UIDim2 position2 = UIDim2(0.25f + vertex2.x / 2, 0, 0.25f + vertex2.y / 2, 0);
            UIDim2 midPos = UIDim2(0.25f + (vertex1.x / 2 + vertex2.x / 2) / 2, 0, 0.25f + (vertex1.y / 2 + vertex2.y / 2) / 2, 0);

            float dx = vertex2.x - vertex1.x;
            float dy = vertex2.y - vertex1.y;
            float len = std::sqrt(dx*dx + dy*dy);
            if (len == 0) continue; // Skip degenerate edge
            dx /= len;
            dy /= len;
            float px = -dy;
            float py = dx;
            px *= 1.0f;
            py *= 1.0f;

            UIDim2 offset1(0.0f, px + 5, 0.0f, py + 5);
            UIDim2 offset2(0.0f, -px + 5, 0.0f, -py + 5);

            float blue = static_cast<float>(edge.id) / static_cast<float>(obj.nextEdgeId - 1);

            window->renderer.drawQuad(
                position1 + offset1,
                position1 + offset2,
                position2 + offset2,
                position2 + offset1,
                bounds, Color4(1.0f - blue, 0.0f, blue) // I want to make the color change depending on how close the edge id is to the last one (at the start its red, at the end its blue)
            );
            window->renderer.drawText(midPos, bounds, std::to_string(edge.id), Theme::font(ThemeFont::Bold), 20, Color4(1.0f - blue / 2, 0.5f, 0.5f + blue / 2));
        }

        for (const Vertex& vertex : obj.vertices) {
            UIDim2 position = UIDim2(0.25f + vertex.x / 2, 0, 0.25f + vertex.y / 2, 0);
            window->renderer.drawRoundedRect(UILayout(UIRect(position, UIDim2(0.0f, 10, 0.0f, 10))), bounds, Color4());
            window->renderer.drawText(position + UIDim2(0.0f, 10, 0.0f, 10), bounds, std::to_string(vertex.id), Theme::font(ThemeFont::Bold), 20, Color4());
        }
    }
}