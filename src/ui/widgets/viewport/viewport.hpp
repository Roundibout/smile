#pragma once

#include <ui/widgets/widget.hpp>

#include <core/object.hpp>

class Viewport : public Widget {
private:
    Object obj;
    float timer = 0.0f;

    float viewScale = 100.0f;
    Vector2 viewPosition = Vector2(0, 0);
    float viewRotation = 0.0f;

    bool movingView = false;
    bool rotatingView = false;
    Vector2 rotatePivot;
    bool zoomingView = false;
    Vector2 zoomCenter;

    Vector2 appliedViewPosition;

    float cosR = std::cosf(viewRotation);
    float sinR = std::sinf(viewRotation);

    Vector2 applyViewTransform(float x, float y);

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
    }

    void update(float deltaTime, const UIBounds& bounds) override;

    void render(const UIBounds& bounds) override;

    bool processWindowInput(WindowInput& input, const UIBounds& bounds) override;
    void observeWindowInput(WindowInput& input, const UIBounds& bounds) override {};
};