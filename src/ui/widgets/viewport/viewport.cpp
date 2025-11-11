#include "viewport.hpp"
#include <core/app.hpp>
#include <window/window.hpp>

Vector2 Viewport::applyViewTransform(float x, float y) {
    return Vector2(
        (x * cosR - y * sinR) * viewScale + appliedViewPosition.x,
        (x * sinR + y * cosR) * viewScale + appliedViewPosition.y
    );
}

Viewport::Viewport(App& app, Window* window, UILayout layout) : Widget(app, window, layout) {
    layout.setCorners(UIDim(0.0f, app.theme.getMetricInt(ThemeMetric::PanelCorner)));

    toolBar = std::make_unique<CategoryToolBar>(app, window, UILayout(UIDim2(0.0f, 10, 0.0f, 0), UIDim2(0.0f, 60, 1.0f, -50)));

    for (auto& tool : app.extensionRegistry->getTools()) {
        Tool* ptr = tool.get();
        ToolConfig config = ptr->getConfig();
        ToolEntryId entry = toolBar->addTool(config.name, "A");
        toolToToolBarEntry[ptr] = entry;
        toolBarEntryToTool[entry] = ptr;
    }

    toolRegisteredConnection = app.extensionRegistry->onToolRegistered.connect([this](Tool* tool) {
        ToolConfig config = tool->getConfig();
        ToolEntryId entry = toolBar->addTool(config.name, "A");
        toolToToolBarEntry[tool] = entry;
        toolBarEntryToTool[entry] = tool;
    });

    toolRemovedConnection = app.extensionRegistry->onToolRemoved.connect([this](Tool* tool) {
        toolBar->removeTool(toolToToolBarEntry[tool]);
        toolToToolBarEntry.erase(tool);
    });

    toolSelectedConnection = toolBar->onToolSelected.connect([this](ToolEntryId id) {
        Tool* tool = toolBarEntryToTool[id];
        this->app.editor->setSelectedTool(this->app.editor->getMode(), tool);
    });

    globalToolSelectedConnection = app.editor->onToolSelected.connect([this](Tool* tool, Editor::Mode mode) { // This does get called when this viewport selects a tool, but whatever
        toolBar->selectTool(toolToToolBarEntry[tool]);
    });
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

void Viewport::drawCanvas(Canvas* canvas, const UIBounds& bounds) {
    Rect rect = canvas->getSize();

    Vector2 applied1 = applyViewTransform(rect.position.x, rect.position.y);
    Vector2 applied2 = applyViewTransform(rect.position.x + rect.size.x, rect.position.y);
    Vector2 applied3 = applyViewTransform(rect.position.x + rect.size.x, rect.position.y + rect.size.y);
    Vector2 applied4 = applyViewTransform(rect.position.x, rect.position.y + rect.size.y);

    UIDim2 position1 = UIDim2(0.0f, applied1.x, 0.0f, applied1.y);
    UIDim2 position2 = UIDim2(0.0f, applied2.x, 0.0f, applied2.y);
    UIDim2 position3 = UIDim2(0.0f, applied3.x, 0.0f, applied3.y);
    UIDim2 position4 = UIDim2(0.0f, applied4.x, 0.0f, applied4.y);

    window->renderer.drawQuad(position1, position2, position3, position4, bounds, canvas->getColor());
}

void Viewport::drawObject(Object* obj, const UIBounds& bounds) {
    if (!computedRender) {
        window->renderer.drawText(UIDim2(0.0f, 10, 1.0f, -30), bounds, "Viewport", app.theme.getFont(ThemeFont::Regular), 20, Color4(1.0f, 1.0f, 1.0f, 0.5f));

        for (const Line& line : obj->lines) {
            const Point* point1 = obj->getPoint(line.point1);
            const Point* point2 = obj->getPoint(line.point2);
            Vector2 applied1 = applyViewTransform(point1->x, point1->y);
            Vector2 applied2 = applyViewTransform(point2->x, point2->y);
            UIDim2 position1 = UIDim2(0.0f, applied1.x, 0.0f, applied1.y);
            UIDim2 position2 = UIDim2(0.0f, applied2.x, 0.0f, applied2.y);

            window->renderer.drawLine(
                position1,
                position2,
                bounds, 
                Color4(0.0f, 1.0f, 0.0f), 
                2.0f
            );
        }

        for (const Point& point : obj->points) {
            Vector2 applied = applyViewTransform(point.x, point.y);
            UIDim2 position = UIDim2(0.0f, applied.x, 0.0f, applied.y);
            UILayout pointLayout = UILayout(UIRect(position + UIDim2(0.0f, -5, 0.0f, -5), UIDim2(0.0f, 10, 0.0f, 10)));
            pointLayout.setCorners(UIDim(1.0f, 0));
            window->renderer.drawRoundedRect(pointLayout, bounds, Color4(0.0f, 1.0f, 0.0f));
            //window->renderer.drawText(position + UIDim2(0.0f, 5, 0.0f, 5), bounds, std::to_string(point.id), Theme::font(ThemeFont::Bold), 20, Color4());
        }
    } else {
        window->renderer.drawText(UIDim2(0.0f, 10, 1.0f, -30), bounds, "Viewport (Computed)", app.theme.getFont(ThemeFont::Regular), 20, Color4(1.0f, 1.0f, 1.0f, 0.5f));

        for (const Face& face : obj->faces) {

            float blue = static_cast<float>(face.id) / static_cast<float>(obj->nextFaceId - 1);

            for (const Triangle& triangle : face.triangles) {
                const Vertex& vertex1 = obj->vertices[triangle.vertex1];
                const Vertex& vertex2 = obj->vertices[triangle.vertex2];
                const Vertex& vertex3 = obj->vertices[triangle.vertex3];
                Vector2 applied1 = applyViewTransform(vertex1.x, vertex1.y);
                Vector2 applied2 = applyViewTransform(vertex2.x, vertex2.y);
                Vector2 applied3 = applyViewTransform(vertex3.x, vertex3.y);
                UIDim2 position1 = UIDim2(0.0f, applied1.x, 0.0f, applied1.y);
                UIDim2 position2 = UIDim2(0.0f, applied2.x, 0.0f, applied2.y);
                UIDim2 position3 = UIDim2(0.0f, applied3.x, 0.0f, applied3.y);

                window->renderer.drawTriangle(position1, position2, position3, bounds, Color4(1.0f - blue, 0.0f, 0.0f + blue, 0.25f));
            }
        }

        for (const Edge& edge : obj->edges) {
            const Vertex& vertex1 = obj->vertices[edge.start];
            const Vertex& vertex2 = obj->vertices[edge.end];
            Vector2 applied1 = applyViewTransform(vertex1.x, vertex1.y);
            Vector2 applied2 = applyViewTransform(vertex2.x, vertex2.y);
            UIDim2 position1 = UIDim2(0.0f, applied1.x, 0.0f, applied1.y);
            UIDim2 position2 = UIDim2(0.0f, applied2.x, 0.0f, applied2.y);
            UIDim2 midPos = UIDim2(0.0f, (position1.x.offset + position2.x.offset) / 2, 0.0f, (position1.y.offset + position2.y.offset) / 2);

            float blue = static_cast<float>(edge.id) / static_cast<float>(obj->nextEdgeId - 1);

            window->renderer.drawLine(
                position1,
                position2,
                bounds, 
                Color4(1.0f - blue, 0.0f, blue), 
                4.0f
            );
            //window->renderer.drawText(midPos - UIDim2(0.0f, 10, 0.0f, 10), bounds, std::to_string(edge.id), Theme::font(ThemeFont::Bold), 20, Color4(1.0f - blue / 2, 0.5f, 0.5f + blue / 2));
        }

        for (const Vertex& vertex : obj->vertices) {
            Vector2 applied = applyViewTransform(vertex.x, vertex.y);
            UIDim2 position = UIDim2(0.0f, applied.x, 0.0f, applied.y);
            UILayout vertexLayout = UILayout(UIRect(position + UIDim2(0.0f, -5, 0.0f, -5), UIDim2(0.0f, 10, 0.0f, 10)));
            vertexLayout.setCorners(UIDim(1.0f, 0));

            float blue = static_cast<float>(vertex.id) / static_cast<float>(obj->nextVertexId - 1);

            window->renderer.drawRoundedRect(vertexLayout, bounds, Color4(1.0f - blue, 0.0f, blue));
            //window->renderer.drawText(position + UIDim2(0.0f, 5, 0.0f, 5), bounds, std::to_string(vertex.id), Theme::font(ThemeFont::Bold), 20, Color4());
        }
    }
}

void Viewport::drawRotationCursor(const AbsoluteLayout& appliedLayout, const UIBounds& bounds) {
    window->renderer.enableSubpixel();

    Vector2 delta = rotatePosition - rotatePivot;

    Color4 color = app.theme.getColor(ThemeColor::Cursor);
    Color4 strokeColor = app.theme.getColor(ThemeColor::CursorStroke);

    if (tooCloseToRotate) color = app.theme.getColor(ThemeColor::Invalid);

    window->renderer.drawStrokeDottedLine(
        UIDim2(0.5f, rotatePosition.x, 0.5f, rotatePosition.y),
        UIDim2(0.5f, rotatePivot.x, 0.5f, rotatePivot.y),
        bounds, 
        color, 
        2, 4, 5, 2,
        strokeColor
    );

    float angle = std::atan2(delta.x, -delta.y);
    float cos = std::cosf(angle);
    float sin = std::sinf(angle);

    Vector2 offset(8, 0);
    offset = Vector2(
        (offset.x * cos - offset.y * sin),
        (offset.x * sin + offset.y * cos)
    );
    
    window->renderer.drawStrokeArrow(
        UIDim2(0.5f, rotatePosition.x + offset.x - appliedLayout.rect.size.x * rotateMirrors.x, 0.5f, rotatePosition.y + offset.y - appliedLayout.rect.size.y * rotateMirrors.y), 
        bounds, 
        angle - PI / 2, 
        color, 
        16, 10, 2, 2,
        strokeColor
    );
    window->renderer.drawStrokeArrow(
        UIDim2(0.5f, rotatePosition.x - offset.x - appliedLayout.rect.size.x * rotateMirrors.x, 0.5f, rotatePosition.y - offset.y - appliedLayout.rect.size.y * rotateMirrors.y), 
        bounds, 
        angle + PI / 2, 
        color, 
        16, 10, 2, 2, 
        strokeColor
    );

    window->renderer.disableSubpixel();
}

void Viewport::drawRotationIndicator(const UIBounds& bounds) {
    window->renderer.enableSubpixel();

    // Rotation indicator
    Vector2 rotationIndicatorBL(
        (-50 * cosR - -50 * sinR),
        (-50 * sinR + -50 * cosR)
    );
    Vector2 rotationIndicatorBR(
        (50 * cosR - -50 * sinR),
        (50 * sinR + -50 * cosR)
    );
    Vector2 rotationIndicatorTL(
        (-50 * cosR - 50 * sinR),
        (-50 * sinR + 50 * cosR)
    );
    Vector2 rotationIndicatorTR(
        (50 * cosR - 50 * sinR),
        (50 * sinR + 50 * cosR)
    );
    window->renderer.drawQuad(
        UIDim2(1.0f, -100 + rotationIndicatorBL.x, 1.0f, -100 + rotationIndicatorBL.y), 
        UIDim2(1.0f, -100 + rotationIndicatorBR.x, 1.0f, -100 + rotationIndicatorBR.y), 
        UIDim2(1.0f, -100 + rotationIndicatorTR.x, 1.0f, -100 + rotationIndicatorTR.y), 
        UIDim2(1.0f, -100 + rotationIndicatorTL.x, 1.0f, -100 + rotationIndicatorTL.y), 
        bounds, 
        app.theme.getColor(ThemeColor::ViewportRotationIndicator)
    );

    Vector2 rotationIndicatorOffset(0, 10);
    rotationIndicatorOffset = Vector2(
        (rotationIndicatorOffset.x * cosR - rotationIndicatorOffset.y * sinR),
        (rotationIndicatorOffset.x * sinR + rotationIndicatorOffset.y * cosR)
    );
    window->renderer.drawSolidArrow(UIDim2(1.0f, -100 + rotationIndicatorOffset.x, 1.0f, -100 + rotationIndicatorOffset.y), bounds, viewRotation, app.theme.getColor(ThemeColor::ViewportRotationIndicatorArrow), Vector2(15, 15), Vector2(30, 15));

    window->renderer.disableSubpixel();
}

void Viewport::render(const UIBounds& bounds) {
    Document* document = app.documents.getCurrentDocument(); // Get current document

    layout.setCorners(UIDim(0.0f, app.theme.getMetricInt(ThemeMetric::PanelCorner)));
    window->renderer.drawRoundedRect(layout, bounds, app.theme.getColor(ThemeColor::ViewportBackground)); // Draw background (TODO: allow the panel background behind to be changed)

    window->renderer.beginStencil();
    window->renderer.drawRoundedRect(layout, bounds);
    window->renderer.useStencil();

    AbsoluteLayout appliedLayout = window->renderer.resolveLayout(layout, bounds);
    appliedViewPosition = Vector2(appliedLayout.rect.size.x / 2 + viewPosition.x, appliedLayout.rect.size.y / 2 + viewPosition.y);

    cosR = std::cosf(viewRotation);
    sinR = std::sinf(viewRotation);

    // Draw document's canvases and objects

    window->renderer.enableSubpixel();

    for (std::unique_ptr<Canvas>& canvasPtr : document->canvases) {
        Canvas* canvas = canvasPtr.get();

        drawCanvas(canvas, bounds);
    }

    for (std::unique_ptr<Instance>& instancePtr : document->instances) {
        Instance* instance = instancePtr.get();

        if (Object* object = dynamic_cast<Object*>(instance)) {
            drawObject(object, bounds);
        }
    }

    window->renderer.disableSubpixel();
    
    if (rotatingView) {
        // If rotating the view, draw the rotation cursor
        drawRotationCursor(appliedLayout, bounds);
    }

    // Draw viewport UI
    drawRotationIndicator(bounds);

    // Toolbar
    toolBar->render(bounds);

    window->renderer.endStencil();
}

bool Viewport::processWindowInput(WindowInput& input, const UIBounds& bounds) {
    AbsoluteLayout resolved = window->renderer.resolveLayout(layout, bounds);

    // Viewport action moves and ends
    if (input.type == WindowInputType::MouseMove) {
        if (movingView) {
            viewPosition = Vector2(viewPosition.x + window->renderer.divide(input.mouse.delta.x), viewPosition.y + window->renderer.divide(input.mouse.delta.y));

            if (!UITools::isPointOverRect(input.mouse.position, resolved)) {
                window->setMousePosition(UITools::mirrorPointAcrossRect(input.mouse.position, resolved));
            }

            window->renderer.dirty();
            return true;
        } else if (rotatingView) {
            rotatePosition = Vector2(
                input.mouse.position.x - resolved.rect.position.x - resolved.rect.size.x / 2 + resolved.rect.size.x * rotateMirrors.x, 
                input.mouse.position.y - resolved.rect.position.y - resolved.rect.size.y / 2 + resolved.rect.size.y * rotateMirrors.y
            );
            
            if (!UITools::isPointOverRect(input.mouse.position, resolved)) {
                if (input.mouse.position.x > resolved.rect.position.x + resolved.rect.size.x) {
                    rotateMirrors.x += 1;
                } else if (input.mouse.position.x < resolved.rect.position.x) {
                    rotateMirrors.x -= 1;
                }
                if (input.mouse.position.y > resolved.rect.position.y + resolved.rect.size.y) {
                    rotateMirrors.y += 1;
                } else if (input.mouse.position.y < resolved.rect.position.y) {
                    rotateMirrors.y -= 1;
                }
                Vector2 newMousePos = UITools::mirrorPointAcrossRect(input.mouse.position, resolved);
                window->setMousePosition(newMousePos);
            }   

            float dx = rotatePosition.x - rotatePivot.x;
            float dy = rotatePosition.y - rotatePivot.y;

            if (sqrtf(dx*dx + dy*dy) < window->renderer.scale(30.0f)) {
                if (!tooCloseToRotate) {
                    lastRotation = viewRotation;
                }
                tooCloseToRotate = true;
                window->renderer.dirty();
                return true;
            }

            float oldRotation = viewRotation;
            float mouseRotation = atan2f(rotatePosition.y - rotatePivot.y, rotatePosition.x - rotatePivot.x) - PI / 2;

            if (tooCloseToRotate) {
                firstRotation = mouseRotation;
            }
            tooCloseToRotate = false;

            viewRotation = mouseRotation - firstRotation + lastRotation;

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
            if (!UITools::isPointOverRect(input.mouse.position, resolved)) {
                window->setMousePosition(UITools::mirrorPointAcrossRect(input.mouse.position, resolved));
            }

            float oldScale = viewScale;
            float zoom = input.mouse.delta.y * 0.005f;

            if (zoom > 0) {
                viewScale = std::clamp(viewScale * (1.0f + abs(zoom)), MIN_SCALE, MAX_SCALE);
            } else if (zoom < 0) {
                viewScale = std::clamp(viewScale / (1.0f + abs(zoom)), MIN_SCALE, MAX_SCALE);
            }

            float scaleFactor = viewScale / oldScale;

            // Adjust the camera so zoom focuses on the center of zooming
            viewPosition = (viewPosition - zoomCenter) * scaleFactor + zoomCenter;

            window->renderer.dirty();
            return true;
        }
    } else if (input.type == WindowInputType::MouseButtonUp) {
        if (input.mouse.button == MouseButton::Middle) {
            if (movingView) {
                movingView = false;
                window->releaseCapture();
                return true;
            } else if (rotatingView) {
                rotatingView = false;
                window->renderer.dirty();
                window->releaseCapture();
                window->showCursor();
                return true;
            } else if (zoomingView) {
                zoomingView = false;
                window->releaseCapture();
                return true;
            }
        }
    }

    // UI
    if (toolBar->processWindowInput(input, bounds)) return true;

    // Viewport action starts
    if (input.type == WindowInputType::MouseScroll) {
        if (!movingView && !rotatingView && UITools::isPointOverRoundedRect(input.mouse.position, resolved)) {
            if (window->isKeyDown(KeyCode::Shift)) {
                if (rotatingView) return true;

                Vector2 mousePos(input.mouse.position.x - resolved.rect.position.x - resolved.rect.size.x / 2, input.mouse.position.y - resolved.rect.position.y - resolved.rect.size.y / 2);   

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
                if (zoomingView) return true;

                Vector2 mousePos(input.mouse.position.x - resolved.rect.position.x - resolved.rect.size.x / 2, input.mouse.position.y - resolved.rect.position.y - resolved.rect.size.y / 2);

                float oldScale = viewScale;

                if (input.mouse.scroll.y > 0) {
                    viewScale = std::clamp(viewScale * 1.1f, MIN_SCALE, MAX_SCALE);
                } else if (input.mouse.scroll.y < 0) {
                    viewScale = std::clamp(viewScale / 1.1f, MIN_SCALE, MAX_SCALE);
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
            if (UITools::isPointOverRoundedRect(input.mouse.position, resolved)) {
                if (window->isKeyDown(KeyCode::Shift)) {
                    rotatingView = true;
                    tooCloseToRotate = true;
                    lastRotation = viewRotation;
                    rotatePivot = Vector2(input.mouse.position.x - resolved.rect.position.x - resolved.rect.size.x / 2, input.mouse.position.y - resolved.rect.position.y - resolved.rect.size.y / 2);
                    rotatePosition = rotatePivot;
                    rotateMirrors = Vector2();
                    window->hideCursor();
                    Logger::print("Rotating");
                } else if (window->isKeyDown(KeyCode::Control)) {
                    zoomingView = true;
                    zoomCenter = Vector2(input.mouse.position.x - resolved.rect.position.x - resolved.rect.size.x / 2, input.mouse.position.y - resolved.rect.position.y - resolved.rect.size.y / 2);
                    Logger::print("Zooming");
                } else {
                    movingView = true;
                    Logger::print("Moving");
                }
                window->renderer.dirty();
                window->setCapture();
                return true;
            }
        }
    }
    return false;
}