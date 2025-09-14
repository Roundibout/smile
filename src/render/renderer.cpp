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

void Renderer::beginFrame() {
    impl->beginFrame();
}

void Renderer::drawRect(const Vector2& position, const Vector2& size, const Color4& color) {
    impl->drawRect(position, size, color);
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