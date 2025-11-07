#pragma once

#include <core/viewport_manager.hpp>
#include <core/document_manager.hpp>

#include <ui/widgets/widget.hpp>

#include <document/object.hpp>

constexpr float MAX_SCALE = 100000.0f;
constexpr float MIN_SCALE = 0.1f;

class Viewport : public Widget {
private:
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

    // Debug
    float timer = 0.0f;
    bool computedRender = false;
public:
    Viewport(Window* window, UILayout layout) : Widget(window, layout) {
        layout.setCorners(UIDim(0.0f, Theme::metricInt(ThemeMetric::PanelCorner)));
    }

    void update(float deltaTime, const UIBounds& bounds) override;

    void render(const UIBounds& bounds) override;

    bool processWindowInput(WindowInput& input, const UIBounds& bounds) override;
    void observeWindowInput(WindowInput& input, const UIBounds& bounds) override {};
};