/*
File:
    renderer_impl.hpp
Authors:
    Lucas
Purpose:
    Definition of the Renderer implementation class
License:
    MIT (see LICENSE file)
*/

#pragma once

#include <datatypes/vector2.hpp>
#include <datatypes/color4.hpp>
#include <datatypes/ui_types.hpp>

#include <window/window_impl.hpp>

class RendererImpl {
protected:
    WindowImpl* window;
    float currentScale = 1.0f;
    bool subpixelEnabled = false;
    bool stencilActive = false;
public:
    RendererImpl(WindowImpl* w);

    float scale(float value) {return value * currentScale;}
    float divide(float value) {return value / currentScale;}
    Vector2 resolvePosition(const UIDim2& position, const UIBounds& bounds, bool subpixel = false);
    AbsoluteLayout resolveLayout(const UILayout& layout, const UIBounds& bounds, bool subpixel = false);
    UIBounds applyLayout(const UIBounds& bounds, const UILayout& layout);

    virtual void beginFrame(float scale) = 0;

    virtual void drawTriangle(const UIDim2& position1, const UIDim2& position2, const UIDim2& position3, const UIBounds& bounds, const Color4& color = Color4()) = 0;
    virtual void drawQuad(const UIDim2& position1, const UIDim2& position2, const UIDim2& position3, const UIDim2& position4, const UIBounds& bounds, const Color4& color = Color4()) = 0;
    virtual void drawRect(const UILayout& layout, const UIBounds& bounds, const Color4& color) = 0;
    virtual void drawStrokeRect(const UILayout& layout, const UIBounds& bounds, const Color4& color, int stroke, const Color4& strokeColor, const UIStrokeAlignment& strokeAlignment) = 0;
    virtual void drawRoundedRect(const UILayout& layout, const UIBounds& bounds, const Color4& color) = 0;
    virtual void drawRoundedStrokeRect(const UILayout& layout, const UIBounds& bounds, const Color4& color, int stroke, const Color4& strokeColor, const UIStrokeAlignment& strokeAlignment) = 0;
    virtual void drawText(const UIDim2& position, const UIBounds& bounds, const std::string& text, const std::string& path, int size, const Color4& color) = 0;

    void enableSubpixel();
    void disableSubpixel();

    virtual void beginStencil() = 0;
    virtual void useStencil() = 0;
    virtual void endStencil() = 0;

    virtual void endFrame() = 0;
};