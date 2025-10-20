#include "viewport.hpp"
#include <window/window.hpp>

Vector2 Viewport::applyViewTransform(float x, float y) {
    return Vector2(
        (x * cosR - y * sinR) * viewScale + appliedViewPosition.x,
        (x * sinR + y * cosR) * viewScale + appliedViewPosition.y
    );
}

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

    window->renderer.beginStencil();
    window->renderer.drawRoundedRect(layout, bounds);
    window->renderer.useStencil();

    AbsoluteLayout appliedLayout = window->renderer.resolveLayout(layout, bounds);
    appliedViewPosition = Vector2(appliedLayout.rect.size.x / 2 + viewPosition.x, appliedLayout.rect.size.y / 2 + viewPosition.y);

    cosR = std::cosf(viewRotation);
    sinR = std::sinf(viewRotation);

    if (!computedRender) {
        window->renderer.drawText(UIDim2(0.0f, 10, 1.0f, -30), bounds, "Viewport", Theme::font(ThemeFont::Regular), 20, Color4(1.0f, 1.0f, 1.0f, 0.5f));

        for (const Line& line : obj.lines) {
            const Point* point1 = obj.getPoint(line.point1);
            const Point* point2 = obj.getPoint(line.point2);
            Vector2 applied1 = applyViewTransform(point1->x, point1->y);
            Vector2 applied2 = applyViewTransform(point2->x, point2->y);
            UIDim2 position1 = UIDim2(0.0f, applied1.x, 0.0f, applied1.y);
            UIDim2 position2 = UIDim2(0.0f, applied2.x, 0.0f, applied2.y);

            float dx = position2.x.offset - position1.x.offset;
            float dy = position2.y.offset - position1.y.offset;
            float len = std::sqrt(dx*dx + dy*dy);
            if (len == 0) continue; // Skip degenerate line
            dx /= len;
            dy /= len;
            float px = -dy;
            float py = dx;
            px *= 1.0f;
            py *= 1.0f;

            UIDim2 offset1(0.0f, px, 0.0f, py);
            UIDim2 offset2(0.0f, -px, 0.0f, -py);

            window->renderer.drawQuad(
                position1 + offset1,
                position1 + offset2,
                position2 + offset2,
                position2 + offset1,
                bounds, Color4()
            );
        }

        for (const Point& point : obj.points) {
            Vector2 applied = applyViewTransform(point.x, point.y);
            UIDim2 position = UIDim2(0.0f, applied.x, 0.0f, applied.y);
            UILayout pointLayout = UILayout(UIRect(position + UIDim2(0.0f, -5, 0.0f, -5), UIDim2(0.0f, 10, 0.0f, 10)));
            pointLayout.setCorners(UIDim(1.0f, 0));
            window->renderer.drawRoundedRect(pointLayout, bounds, Color4());
            //window->renderer.drawText(position + UIDim2(0.0f, 5, 0.0f, 5), bounds, std::to_string(point.id), Theme::font(ThemeFont::Bold), 20, Color4());
        }
    } else {
        window->renderer.drawText(UIDim2(0.0f, 10, 1.0f, -30), bounds, "Viewport (Computed)", Theme::font(ThemeFont::Regular), 20, Color4(1.0f, 1.0f, 1.0f, 0.5f));

        for (const Face& face : obj.faces) {

            float blue = static_cast<float>(face.id) / static_cast<float>(obj.nextFaceId - 1);

            for (const Triangle& triangle : face.triangles) {
                const Vertex& vertex1 = obj.vertices[triangle.vertex1];
                const Vertex& vertex2 = obj.vertices[triangle.vertex2];
                const Vertex& vertex3 = obj.vertices[triangle.vertex3];
                Vector2 applied1 = applyViewTransform(vertex1.x, vertex1.y);
                Vector2 applied2 = applyViewTransform(vertex2.x, vertex2.y);
                Vector2 applied3 = applyViewTransform(vertex3.x, vertex3.y);
                UIDim2 position1 = UIDim2(0.0f, applied1.x, 0.0f, applied1.y);
                UIDim2 position2 = UIDim2(0.0f, applied2.x, 0.0f, applied2.y);
                UIDim2 position3 = UIDim2(0.0f, applied3.x, 0.0f, applied3.y);

                window->renderer.drawTriangle(position1, position2, position3, bounds, Color4(1.0f - blue, 0.0f, 0.0f + blue, 0.25f));
            }
        }

        for (const Edge& edge : obj.edges) {
            const Vertex& vertex1 = obj.vertices[edge.start];
            const Vertex& vertex2 = obj.vertices[edge.end];
            Vector2 applied1 = applyViewTransform(vertex1.x, vertex1.y);
            Vector2 applied2 = applyViewTransform(vertex2.x, vertex2.y);
            UIDim2 position1 = UIDim2(0.0f, applied1.x, 0.0f, applied1.y);
            UIDim2 position2 = UIDim2(0.0f, applied2.x, 0.0f, applied2.y);
            UIDim2 midPos = UIDim2(0.0f, (position1.x.offset + position2.x.offset) / 2, 0.0f, (position1.y.offset + position2.y.offset) / 2);

            float dx = position2.x.offset - position1.x.offset;
            float dy = position2.y.offset - position1.y.offset;
            float len = std::sqrt(dx*dx + dy*dy);
            if (len == 0) continue; // Skip degenerate edge
            dx /= len;
            dy /= len;
            float px = -dy;
            float py = dx;
            px *= 1.0f;
            py *= 1.0f;

            UIDim2 offset1(0.0f, px, 0.0f, py);
            UIDim2 offset2(0.0f, -px, 0.0f, -py);

            float blue = static_cast<float>(edge.id) / static_cast<float>(obj.nextEdgeId - 1);

            window->renderer.drawQuad(
                position1 + offset1,
                position1 + offset2,
                position2 + offset2,
                position2 + offset1,
                bounds, Color4(1.0f - blue, 0.0f, blue)
            );
            //window->renderer.drawText(midPos - UIDim2(0.0f, 10, 0.0f, 10), bounds, std::to_string(edge.id), Theme::font(ThemeFont::Bold), 20, Color4(1.0f - blue / 2, 0.5f, 0.5f + blue / 2));
        }

        for (const Vertex& vertex : obj.vertices) {
            Vector2 applied = applyViewTransform(vertex.x, vertex.y);
            UIDim2 position = UIDim2(0.0f, applied.x, 0.0f, applied.y);
            UILayout vertexLayout = UILayout(UIRect(position + UIDim2(0.0f, -5, 0.0f, -5), UIDim2(0.0f, 10, 0.0f, 10)));
            vertexLayout.setCorners(UIDim(1.0f, 0));
            window->renderer.drawRoundedRect(vertexLayout, bounds, Color4());
            //window->renderer.drawText(position + UIDim2(0.0f, 5, 0.0f, 5), bounds, std::to_string(vertex.id), Theme::font(ThemeFont::Bold), 20, Color4());
        }
    }

    window->renderer.endStencil();
}

bool Viewport::processWindowInput(WindowInput& input, const UIBounds& bounds) {
    AbsoluteLayout applied = window->renderer.resolveLayout(layout, bounds);
    if (input.type == WindowInputType::MouseScroll) {
        if (!movingView && !rotatingView && UITools::isPointOverRoundedRect(input.mouse.position, applied)) {
            if (window->isKeyDown(KeyCode::Shift)) {
                Vector2 mousePos(input.mouse.position.x - applied.rect.position.x - applied.rect.size.x / 2, input.mouse.position.y - applied.rect.position.y - applied.rect.size.y / 2);   

                float oldRotation = viewRotation;
                if (input.mouse.scroll.y > 0) {
                    viewRotation -= PI / 24;
                } else if (input.mouse.scroll.y < 0) {
                    viewRotation += PI / 24;
                }

                cosR = std::cosf(viewRotation);
                sinR = std::sinf(viewRotation);

                float dR = viewRotation - oldRotation;
                float cosD = std::cosf(dR);
                float sinD = std::sinf(dR);

                // Rotate view position around the mouse pivot
                Vector2 offset = viewPosition - mousePos;
                Vector2 rotatedOffset{
                    offset.x * cosD - offset.y * sinD,
                    offset.x * sinD + offset.y * cosD
                };
                viewPosition = mousePos + rotatedOffset;

                Logger::print("Rotated by increment");
            } else {
                Vector2 mousePos(input.mouse.position.x - applied.rect.position.x - applied.rect.size.x / 2, input.mouse.position.y - applied.rect.position.y - applied.rect.size.y / 2);

                float oldScale = viewScale;

                if (input.mouse.scroll.y > 0) {
                    viewScale *= 1.1f;
                } else if (input.mouse.scroll.y < 0) {
                    viewScale /= 1.1f;
                }

                float scaleFactor = viewScale / oldScale;

                // Adjust the camera so zoom focuses on mouse point
                viewPosition = (viewPosition - mousePos) * scaleFactor + mousePos;
                Logger::print("Zoomed by increment");
            }

            window->renderer.dirty();
            return true;
        }
    } else if (input.type == WindowInputType::MouseButtonDown) {
        if (input.mouse.button == MouseButton::Middle) {
            if (UITools::isPointOverRoundedRect(input.mouse.position, applied)) {
                if (window->isKeyDown(KeyCode::Shift)) {
                    rotatingView = true;
                    rotatePivot = Vector2(input.mouse.position.x - applied.rect.position.x - applied.rect.size.x / 2, input.mouse.position.y - applied.rect.position.y - applied.rect.size.y / 2);
                    Logger::print("Rotating");
                } else if (window->isKeyDown(KeyCode::Control)) {
                    zoomingView = true;
                    zoomCenter = Vector2(input.mouse.position.x - applied.rect.position.x - applied.rect.size.x / 2, input.mouse.position.y - applied.rect.position.y - applied.rect.size.y / 2);
                    Logger::print("Zooming");
                } else {
                    movingView = true;
                    Logger::print("Moving");
                }
                window->setCapture();
                return true;
            }
        }
    } else if (input.type == WindowInputType::MouseButtonUp) {
        if (input.mouse.button == MouseButton::Middle) {
            if (movingView) {
                movingView = false;
                window->releaseCapture();
                return true;
            } else if (rotatingView) {
                rotatingView = false;
                window->releaseCapture();
                return true;
            } else if (zoomingView) {
                zoomingView = false;
                window->releaseCapture();
                return true;
            }
        }
    } else if (input.type == WindowInputType::MouseMove) {
        if (movingView) {
            viewPosition = Vector2(viewPosition.x + window->renderer.divide(input.mouse.delta.x), viewPosition.y + window->renderer.divide(input.mouse.delta.y));

            if (!UITools::isPointOverRect(input.mouse.position, applied)) {
                window->setMousePosition(UITools::mirrorPointAcrossRect(input.mouse.position, applied));
            }

            window->renderer.dirty();
            return true;
        } else if (rotatingView) {
            if (!UITools::isPointOverRect(input.mouse.position, applied)) {
                window->setMousePosition(UITools::mirrorPointAcrossRect(input.mouse.position, applied));
            }

            float oldRotation = viewRotation;
            viewRotation += PI * input.mouse.delta.y * -0.001f;

            cosR = std::cosf(viewRotation);
            sinR = std::sinf(viewRotation);

            float dR = viewRotation - oldRotation;
            float cosD = std::cosf(dR);
            float sinD = std::sinf(dR);

            // Rotate view position around the rotate pivot
            Vector2 offset = viewPosition - rotatePivot;
            Vector2 rotatedOffset{
                offset.x * cosD - offset.y * sinD,
                offset.x * sinD + offset.y * cosD
            };
            viewPosition = rotatePivot + rotatedOffset;

            window->renderer.dirty();
            return true;
        } else if (zoomingView) {
            if (!UITools::isPointOverRect(input.mouse.position, applied)) {
                window->setMousePosition(UITools::mirrorPointAcrossRect(input.mouse.position, applied));
            }

            float oldScale = viewScale;
            float zoom = input.mouse.delta.y * 0.005f;

            if (zoom > 0) {
                viewScale *= 1.0f + abs(zoom);
            } else if (zoom < 0) {
                viewScale /= 1.0f + abs(zoom);
            }

            float scaleFactor = viewScale / oldScale;

            // Adjust the camera so zoom focuses on the center of zooming
            viewPosition = (viewPosition - zoomCenter) * scaleFactor + zoomCenter;

            window->renderer.dirty();
        }
    }
    return false;
}