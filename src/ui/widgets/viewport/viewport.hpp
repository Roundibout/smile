#pragma once

#include <unordered_map>

#include <core/signal.hpp>
#include <core/editor.hpp>

#include <ui/widgets/widget.hpp>

#include <ui/widgets/tool/category_tool_bar.hpp>

#include <document/canvas.hpp>
#include <document/object.hpp>

#include <extension/tool.hpp>

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

    void drawCanvas(Canvas* canvas, const UIBounds& bounds);
    void drawCanvasOutline(Canvas* canvas, const UIBounds& bounds); // Separate function so overlapping canvases are visible
    void drawObject(Object* obj, const UIBounds& bounds);
    void drawRotationCursor(const AbsoluteLayout& appliedLayout, const UIBounds& bounds);
    void drawRotationIndicator(const UIBounds& bounds);

    // UI elements
    std::unique_ptr<CategoryToolBar> toolBar;
    std::unordered_map<Tool*, ToolEntryId> toolToToolBarEntry;
    std::unordered_map<ToolEntryId, Tool*> toolBarEntryToTool;
    Signal<Tool*>::Connection toolRegisteredConnection;
    Signal<Tool*>::Connection toolRemovedConnection;
    Signal<ToolEntryId>::Connection toolSelectedConnection;
    Signal<Tool*, Editor::Mode>::Connection globalToolSelectedConnection;

    // Debug
    float timer = 0.0f;
    bool computedRender = false;
public:
    Viewport(App& app, Window* window, UILayout layout);

    void update(float deltaTime, const UIBounds& bounds) override;

    void render(const UIBounds& bounds) override;

    bool processWindowInput(WindowInput& input, const UIBounds& bounds) override;
    void observeWindowInput(WindowInput& input, const UIBounds& bounds) override {};
};