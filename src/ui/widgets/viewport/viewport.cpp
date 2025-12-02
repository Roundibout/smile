#include "viewport.hpp"

#include "core/app.hpp"
#include "window/window.hpp"

Vector2 Viewport::applyViewTransform(float x, float y) {
    return Vector2(
        (x * cosR - y * sinR) * viewScale + appliedViewPosition.x,
        (x * sinR + y * cosR) * viewScale + appliedViewPosition.y
    );
}

Vector2 Viewport::mouseToWorldSpace(Vector2 mousePos) {
    float dx = (mousePos.x - viewPosition.x) / viewScale;
    float dy = (mousePos.y - viewPosition.y) / viewScale;
    return Vector2(
        dx * cosR + dy * sinR,
        -dx * sinR + dy * cosR
    );
}

std::string toolCategoryToString(ToolCategory category) {
    switch (category) {
        case ToolCategory::Select:
            return "Select";
            break;
        case ToolCategory::Transform:
            return "Transform";
            break;
        case ToolCategory::Add:
            return "Add";
            break;
        case ToolCategory::Modify:
            return "Modify";
            break;
        case ToolCategory::Custom:
            return "Custom"; // TODO: make each individual extension get a different category if custom
            break;
    }
    return "Custom";
}

void Viewport::fillToolBar() {
    for (auto& tool : app.extensionRegistry->getTools()) {
        Tool* ptr = tool.get();
        if (tool->getMode() != app.editor->getMode()) continue;

        std::string category = toolCategoryToString(tool->getCategory());
        ToolEntryId entry = toolBar->addTool(tool->getName(), category);
        toolToToolBarEntry[ptr] = entry;
        toolBarEntryToTool[entry] = ptr;

        if (ptr == app.editor->getSelectedTool(app.editor->getMode())) toolBar->selectTool(entry);
    }
}

Viewport::Viewport(
    App& app, 
    Window* window, 
    UILayout layout
) : 
    Widget(app, window, layout) 
{
    layout.setCorners(UIDim(0.0f, app.theme.getMetricInt(ThemeMetric::PanelCorner)));

    toolBar = std::make_unique<CategoryToolBar>(app, window, UILayout(UIDim2(0.0f, 10, 0.0f, 0), UIDim2(0.0f, 60, 1.0f, -50)));

    fillToolBar();

    modeChangedConnection = app.editor->onModeChanged.connect([this](Editor::Mode mode) {
        toolToToolBarEntry.clear();
        toolBarEntryToTool.clear();
        toolBar->clearTools();
        fillToolBar();
    });

    toolRegisteredConnection = app.extensionRegistry->onToolRegistered.connect([this](Tool* tool) {
        std::string category = toolCategoryToString(tool->getCategory());
        ToolEntryId entry = toolBar->addTool(tool->getName(), category);
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
            //computedRender = true;
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

void Viewport::drawCanvasOutline(Canvas* canvas, const UIBounds& bounds) {
    Rect rect = canvas->getSize();

    Vector2 applied1 = applyViewTransform(rect.position.x, rect.position.y);
    Vector2 applied2 = applyViewTransform(rect.position.x + rect.size.x, rect.position.y);
    Vector2 applied3 = applyViewTransform(rect.position.x + rect.size.x, rect.position.y + rect.size.y);
    Vector2 applied4 = applyViewTransform(rect.position.x, rect.position.y + rect.size.y);

    UIDim2 position1 = UIDim2(0.0f, applied1.x, 0.0f, applied1.y);
    UIDim2 position2 = UIDim2(0.0f, applied2.x, 0.0f, applied2.y);
    UIDim2 position3 = UIDim2(0.0f, applied3.x, 0.0f, applied3.y);
    UIDim2 position4 = UIDim2(0.0f, applied4.x, 0.0f, applied4.y);

    window->renderer.drawLine(position1, position2, bounds, Color4(0.0f, 0.0f, 0.0f), 2.0f);
    window->renderer.drawLine(position2, position3, bounds, Color4(0.0f, 0.0f, 0.0f), 2.0f);
    window->renderer.drawLine(position3, position4, bounds, Color4(0.0f, 0.0f, 0.0f), 2.0f);
    window->renderer.drawLine(position4, position1, bounds, Color4(0.0f, 0.0f, 0.0f), 2.0f);
}

void Viewport::drawObjectHandles(Object* obj, const UIBounds& bounds) {
    for (std::unique_ptr<Line>& line : obj->getLines()) {
        std::unique_ptr<Point>& point1 = obj->getPoint(line->point1);
        std::unique_ptr<Point>& point2 = obj->getPoint(line->point2);
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

    for (std::unique_ptr<Point>& point : obj->getPoints()) {
        Vector2 applied = applyViewTransform(point->x, point->y);
        UIDim2 position = UIDim2(0.0f, applied.x, 0.0f, applied.y);
        UILayout pointLayout = UILayout(UIRect(position + UIDim2(0.0f, -5, 0.0f, -5), UIDim2(0.0f, 10, 0.0f, 10)));
        window->renderer.drawRect(pointLayout, bounds, Color4(0.0f, 1.0f, 0.0f));
        //window->renderer.drawText(position + UIDim2(0.0f, 5, 0.0f, 5), bounds, std::to_string(point.id), Theme::font(ThemeFont::Bold), 20, Color4());
    }
}

void Viewport::drawObject(Object* obj, const UIBounds& bounds) {
    // Do stuff later
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

    for (std::unique_ptr<Canvas>& canvasPtr : document->canvases) {
        Canvas* canvas = canvasPtr.get();

        drawCanvasOutline(canvas, bounds);
    }

    for (std::unique_ptr<Instance>& instancePtr : document->instances) {
        Instance* instance = instancePtr.get();

        if (Object* object = dynamic_cast<Object*>(instance)) {
            drawObjectHandles(object, bounds);
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

    // Temporary title
    if (not computedRender) {
        window->renderer.drawText(UIDim2(0.0f, 10, 1.0f, -30), bounds, "Viewport", app.theme.getFont(ThemeFont::Regular), 20, Color4(1.0f, 1.0f, 1.0f, 0.5f));
    } else {
        window->renderer.drawText(UIDim2(0.0f, 10, 1.0f, -30), bounds, "Viewport (Computed)", app.theme.getFont(ThemeFont::Regular), 20, Color4(1.0f, 1.0f, 1.0f, 0.5f));
    }
}

bool Viewport::processWindowInput(WindowInput& input, const UIBounds& bounds) {
    AbsoluteLayout resolved = window->renderer.resolveLayout(layout, bounds);

    // Viewport action moves and ends
    if (input.type == WindowInputType::MouseMove) {
        if (movingView) {
            viewPosition = Vector2(viewPosition.x + window->renderer.divide(input.mouse.delta.x), viewPosition.y + window->renderer.divide(input.mouse.delta.y));

            if (not ui_tools::is_point_over_rect(input.mouse.position, resolved)) {
                window->setMousePosition(ui_tools::mirror_point_across_rect(input.mouse.position, resolved));
            }

            window->renderer.dirty();
            return true;
        } else if (rotatingView) {
            rotatePosition = Vector2(
                input.mouse.position.x - resolved.rect.position.x - resolved.rect.size.x / 2 + resolved.rect.size.x * rotateMirrors.x, 
                input.mouse.position.y - resolved.rect.position.y - resolved.rect.size.y / 2 + resolved.rect.size.y * rotateMirrors.y
            );
            
            if (not ui_tools::is_point_over_rect(input.mouse.position, resolved)) {
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
                Vector2 newMousePos = ui_tools::mirror_point_across_rect(input.mouse.position, resolved);
                window->setMousePosition(newMousePos);
            }   

            float dx = rotatePosition.x - rotatePivot.x;
            float dy = rotatePosition.y - rotatePivot.y;

            if (sqrtf(dx*dx + dy*dy) < window->renderer.scale(30.0f)) {
                if (not tooCloseToRotate) {
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
            if (not ui_tools::is_point_over_rect(input.mouse.position, resolved)) {
                window->setMousePosition(ui_tools::mirror_point_across_rect(input.mouse.position, resolved));
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

    if ((
        input.type == WindowInputType::MouseMove or 
        input.type == WindowInputType::MouseButtonDown or 
        input.type == WindowInputType::MouseButtonUp or 
        input.type == WindowInputType::MouseScroll) 
        and not ui_tools::is_point_over_rounded_rect(input.mouse.position, resolved)
    ) return false;

    // UI
    if (toolBar->processWindowInput(input, bounds)) return true;

    // Viewport action starts
    if (input.type == WindowInputType::MouseScroll) {
        if (not movingView and not rotatingView and ui_tools::is_point_over_rounded_rect(input.mouse.position, resolved)) {
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

                console::print("Rotated by increment");
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
                console::print("Zoomed by increment");
            }

            window->renderer.dirty();
            return true;
        }
    } else if (input.type == WindowInputType::MouseButtonDown) {
        if (input.mouse.button == MouseButton::Middle) {
            if (ui_tools::is_point_over_rounded_rect(input.mouse.position, resolved)) {
                if (window->isKeyDown(KeyCode::Shift)) {
                    rotatingView = true;
                    tooCloseToRotate = true;
                    lastRotation = viewRotation;
                    rotatePivot = Vector2(input.mouse.position.x - resolved.rect.position.x - resolved.rect.size.x / 2, input.mouse.position.y - resolved.rect.position.y - resolved.rect.size.y / 2);
                    rotatePosition = rotatePivot;
                    rotateMirrors = Vector2();
                    window->hideCursor();
                    console::print("Rotating");
                } else if (window->isKeyDown(KeyCode::Control)) {
                    zoomingView = true;
                    zoomCenter = Vector2(input.mouse.position.x - resolved.rect.position.x - resolved.rect.size.x / 2, input.mouse.position.y - resolved.rect.position.y - resolved.rect.size.y / 2);
                    console::print("Zooming");
                } else {
                    movingView = true;
                    console::print("Moving");
                }
                window->renderer.dirty();
                window->setCapture();
                return true;
            }
        }
    }

    // Tools
    if ((input.type == WindowInputType::MouseButtonDown or input.type == WindowInputType::MouseButtonUp) and (input.mouse.button == MouseButton::Left or input.mouse.button == MouseButton::Right)) {
        // Calculate world position of mouse
        Vector2 mousePos(input.mouse.position.x - resolved.rect.position.x - resolved.rect.size.x / 2, input.mouse.position.y - resolved.rect.position.y - resolved.rect.size.y / 2);
        Vector2 worldPos = mouseToWorldSpace(mousePos);

        ToolEventType event;
        if (input.type == WindowInputType::MouseButtonDown) {
            if (input.mouse.button == MouseButton::Left) {
                event = ToolEventType::LeftMouseDown;
            } else if (input.mouse.button == MouseButton::Right) {
                event = ToolEventType::RightMouseDown;
            }
        } else if (input.type == WindowInputType::MouseButtonUp) {
            if (input.mouse.button == MouseButton::Left) {
                event = ToolEventType::LeftMouseUp;
            } else if (input.mouse.button == MouseButton::Right) {
                event = ToolEventType::RightMouseUp;
            }
        }
        app.editor->getSelectedTool(app.editor->getMode())->fireEvent(event, Vector2(worldPos.x, worldPos.y));
    }

    return false;
}