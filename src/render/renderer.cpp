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

void Renderer::endFrame() {
    impl->endFrame();
}