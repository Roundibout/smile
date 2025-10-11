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

Vector2 Renderer::resolvePosition(const UIDim2& position, const UIBounds& bounds) {
    return impl->resolvePosition(position, bounds);
}

AbsoluteLayout Renderer::resolveLayout(const UILayout& layout, const UIBounds& bounds) {
    return impl->resolveLayout(layout, bounds);
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