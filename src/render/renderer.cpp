/*
File:
    renderer.cpp
Authors:
    Lucas
Purpose:
    Implementation of the Renderer class
License:
    MIT (see LICENSE file)
*/

#include "renderer.hpp"

Renderer::Renderer(WindowImpl* window) {
    impl = std::make_unique<RendererGL>(window); // only OpenGL for now (a while probably)
}

Vector2 Renderer::resolvePosition(const UIDim2& position, const UIBounds& bounds, bool subpixel) {
    return impl->resolvePosition(position, bounds, subpixel);
}

AbsoluteLayout Renderer::resolveLayout(const UILayout& layout, const UIBounds& bounds, bool subpixel) {
    return impl->resolveLayout(layout, bounds, subpixel);
}

UIBounds Renderer::applyLayout(const UIBounds& bounds, const UILayout& layout) {
    return impl->applyLayout(bounds, layout);
}

void Renderer::beginFrame(float scale) {
    impl->beginFrame(scale);
}

void Renderer::drawTriangle(const UIDim2& position1, const UIDim2& position2, const UIDim2& position3, const UIBounds& bounds, const Color4& color) {
    impl->drawTriangle(position1, position2, position3, bounds, color);
}

void Renderer::drawQuad(const UIDim2& position1, const UIDim2& position2, const UIDim2& position3, const UIDim2& position4, const UIBounds& bounds, const Color4& color) {
    impl->drawQuad(position1, position2, position3, position4, bounds, color);
}

void Renderer::drawLine(const UIDim2& position1, const UIDim2& position2, const UIBounds& bounds, const Color4& color, float thickness) {
    float dx = position2.x.offset - position1.x.offset;
    float dy = position2.y.offset - position1.y.offset;
    float len = std::sqrt(dx*dx + dy*dy);

    if (len == 0) return; // Skip degenerate line

    dx /= len;
    dy /= len;

    float px = -dy;
    float py = dx;

    px *= thickness / 2;
    py *= thickness / 2;

    UIDim2 offset1(0.0f, px, 0.0f, py);
    UIDim2 offset2(0.0f, -px, 0.0f, -py);

    impl->drawQuad(
        position1 + offset1,
        position1 + offset2,
        position2 + offset2,
        position2 + offset1,
        bounds, color
    );
}

void Renderer::drawDottedLine(const UIDim2& position1, const UIDim2& position2, const UIBounds& bounds, const Color4& color, float thickness, float dotLength, float spacing) {
    float dx = position2.x.offset - position1.x.offset;
    float dy = position2.y.offset - position1.y.offset;
    float len = std::sqrt(dx*dx + dy*dy);

    if (len == 0) return; // Skip degenerate line

    float dirX = dx / len;
    float dirY = dy / len;

    float currentPos = 0.0f;

    while (currentPos < len) {
        float startOffset = currentPos;
        float endOffset = std::min(currentPos + dotLength, len);

        UIDim2 dotStart(
            position1.x.scale,
            position1.x.offset + dirX * startOffset,
            position1.y.scale,
            position1.y.offset + dirY * startOffset
        );

        UIDim2 dotEnd(
            position1.x.scale,
            position1.x.offset + dirX * endOffset,
            position1.y.scale,
            position1.y.offset + dirY * endOffset
        );

        // Draw the dot as a line
        drawLine(dotStart, dotEnd, bounds, color, thickness);

        // Advance to the next dot
        currentPos += dotLength + spacing;
    }
}

void Renderer::drawRect(const UILayout& layout, const UIBounds& bounds, const Color4& color) {
    impl->drawRect(layout, bounds, color);
}

void Renderer::drawStrokeRect(const UILayout& layout, const UIBounds& bounds, const Color4& color, int stroke, const Color4& strokeColor, const UIStrokeAlignment& strokeAlignment) {
    impl->drawStrokeRect(layout, bounds, color, stroke, strokeColor, strokeAlignment);
}

void Renderer::drawRoundedRect(const UILayout& layout, const UIBounds& bounds, const Color4& color) {
    impl->drawRoundedRect(layout, bounds, color);
}

void Renderer::drawRoundedStrokeRect(const UILayout& layout, const UIBounds& bounds, const Color4& color, int stroke, const Color4& strokeColor, const UIStrokeAlignment& strokeAlignment) {
    impl->drawRoundedStrokeRect(layout, bounds, color, stroke, strokeColor, strokeAlignment);
}

void Renderer::drawText(const UIDim2& position, const UIBounds& bounds, const std::string& text, const std::string& path, int size, const Color4& color) {
    impl->drawText(position, bounds, text, path, size, color);
}

void Renderer::enableSubpixel() {
    impl->enableSubpixel();
}

void Renderer::disableSubpixel() {
    impl->disableSubpixel();
}

void Renderer::beginStencil() {
    impl->beginStencil();
}

void Renderer::useStencil() {
    impl->useStencil();
}

void Renderer::endStencil() {
    impl->endStencil();
}

void Renderer::endFrame() {
    impl->endFrame();
}

void Renderer::dirty() {
    dirtied = true;
}

void Renderer::clean() {
    dirtied = false;
}

bool Renderer::isDirty() {
    return dirtied;
}