#pragma once

#include <ui/widgets/widget.hpp>

#include <core/object.hpp>

class Viewport : public Widget {
private:
    Object obj;
public:
    Viewport(Window* window, UILayout layout) : Widget(window, layout), obj(Object("Test")) {
        Id v1 = obj.createPoint(Vector2(0.0f, 0.2f));
        Id v2 = obj.createPoint(Vector2(0.1f, 1.0f));
        Id v3 = obj.createPoint(Vector2(1.0f, 0.8f));
        Id v4 = obj.createPoint(Vector2(0.9f, 0.0f));
        Id v5 = obj.createPoint(Vector2(0.5f, 0.5f));
        obj.createLine(v1, v2);
        obj.createLine(v2, v3);
        obj.createLine(v3, v4);
        obj.createLine(v4, v5);
        obj.createLine(v5, v1);
        obj.compute();
    }

    void update(float deltaTime, const UIBounds& bounds) override {

    }

    void render(const UIBounds& bounds) override;

    bool processWindowInput(WindowInput&, const UIBounds&) override {return false;};
    void observeWindowInput(WindowInput&, const UIBounds&) override {};
};