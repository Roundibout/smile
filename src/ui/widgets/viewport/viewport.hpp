#pragma once

#include <memory>

#include <ui/widgets/widget.hpp>

#include <ui/widgets/tool/category_tool_bar.hpp>

#include <core/object.hpp>

constexpr float MAX_SCALE = 100000.0f;
constexpr float MIN_SCALE = 0.1f;

class Viewport : public Widget {
private:
    Object obj;

    float timer = 0.0f;

    float viewScale = 100.0f;
    Vector2 viewPosition = Vector2(0, 0);
    float viewRotation = 0.0f;

    bool movingView = false;
    bool rotatingView = false;
    bool tooCloseToRotate = false;
    float lastRotation = 0.0f;
    float firstRotation = 0.0f;
    Vector2 rotatePivot;
    Vector2 rotatePosition;
    Vector2 rotateMirrors;
    bool zoomingView = false;
    Vector2 zoomCenter;

    Vector2 appliedViewPosition;

    float cosR = std::cosf(viewRotation);
    float sinR = std::sinf(viewRotation);

    Vector2 applyViewTransform(float x, float y);

    // UI elements

    std::unique_ptr<CategoryToolBar> toolBar;

    // Debug
    bool computedRender = false;
public:
    Viewport(Window* window, UILayout layout) : Widget(window, layout), obj(Object("Test")) {
        layout.setCorners(UIDim(0.0f, Theme::metricInt(ThemeMetric::PanelCorner)));
        Id v1 = obj.createPoint(Vector2(0.0f, 0.0f));
        Id v2 = obj.createPoint(Vector2(50.0f, 100.0f));
        Id v3 = obj.createPoint(Vector2(100.0f, 0.0f));
        obj.createLine(v1, v2);
        obj.createLine(v2, v3);
        obj.createLine(v3, v1);
        obj.compute();

        toolBar = std::make_unique<CategoryToolBar>(window, UILayout(UIDim2(0.0f, 10, 0.0f, 0), UIDim2(0.0f, 60, 1.0f, -50)));
        toolBar->addTool("Select", "Select");
        toolBar->addTool("Move", "Transform");
        toolBar->addTool("Rotate", "Transform");
        toolBar->addTool("Scale", "Transform");
        toolBar->addTool("Rectangle", "Add");
        toolBar->addTool("Ellipse", "Add");
        toolBar->addTool("Pen", "Modify");
        toolBar->addTool("Brush", "Modify");
        toolBar->addTool("Fill", "Modify");
        toolBar->addTool("Knife", "Modify");
    }

    void update(float deltaTime, const UIBounds& bounds) override;

    void render(const UIBounds& bounds) override;

    bool processWindowInput(WindowInput& input, const UIBounds& bounds) override;
    void observeWindowInput(WindowInput& input, const UIBounds& bounds) override {};
};