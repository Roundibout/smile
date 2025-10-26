/*
File:
    renderer.hpp
Authors:
    Lucas
Purpose:
    Definition of the Renderer class
License:
    MIT (see LICENSE file)
*/

#pragma once

#include <memory>
#include <string>

#include <datatypes/ui_types.hpp>

#include <window/window_impl.hpp>
#include <render/renderer_impl.hpp>
#include <render/renderer_gl.hpp>

class Renderer {
private:
    std::unique_ptr<RendererImpl> impl;
    bool dirtied = false;
public:
    Renderer(WindowImpl* window);

    float scale(float value) {return impl->scale(value);}
    float divide(float value) {return impl->divide(value);}
    Vector2 resolvePosition(const UIDim2& position, const UIBounds& bounds);
    AbsoluteLayout resolveLayout(const UILayout& layout, const UIBounds& bounds);
    UIBounds applyLayout(const UIBounds& bounds, const UILayout& layout);

    void beginFrame(float scale);

    void drawTriangle(const UIDim2& position1, const UIDim2& position2, const UIDim2& position3, const UIBounds& bounds, const Color4& color = Color4());
    void drawQuad(const UIDim2& position1, const UIDim2& position2, const UIDim2& position3, const UIDim2& position4, const UIBounds& bounds, const Color4& color = Color4());
    void drawLine(const UIDim2& position1, const UIDim2& position2, const UIBounds& bounds, const Color4& color = Color4(), float thickness = 1.0f);
    void drawDottedLine(const UIDim2& position1, const UIDim2& position2, const UIBounds& bounds, const Color4& color = Color4(), float thickness = 1.0f, float dotLength = 2.0f, float spacing = 2.0f);
    void drawRect(const UILayout& layout, const UIBounds& bounds, const Color4& color = Color4());
    void drawStrokeRect(const UILayout& layout, const UIBounds& bounds, const Color4& color = Color4(), int stroke = 1, const Color4& strokeColor = Color4(0.0f, 0.0f, 0.0f), const UIStrokeAlignment& strokeAlignment = UIStrokeAlignment::Outside);
    void drawRoundedRect(const UILayout& layout, const UIBounds& bounds, const Color4& color = Color4());
    void drawRoundedStrokeRect(const UILayout& layout, const UIBounds& bounds, const Color4& color = Color4(), int stroke = 1, const Color4& strokeColor = Color4(0.0f, 0.0f, 0.0f), const UIStrokeAlignment& strokeAlignment = UIStrokeAlignment::Outside);
    void drawText(const UIDim2& position, const UIBounds& bounds, const std::string& text, const std::string& path, int size, const Color4& color);

    void beginStencil();
    void useStencil();
    void endStencil();

    void endFrame();

    void dirty();
    void clean();
    bool isDirty();
};