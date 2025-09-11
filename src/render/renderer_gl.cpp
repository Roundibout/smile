/*
File:
    renderer_gl.cpp
Authors:
    Lucas
Purpose:
    Implementation of the Renderer implementation class for OpenGL
License:
    MIT (see LICENSE file)
*/

#include "renderer_gl.hpp"

RendererGL::RendererGL(WindowImpl* w) : RendererImpl(w) {
    w->bindGLContext();
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
}

void RendererGL::beginFrame() {
    window->makeGLCurrent();

    // Get and set viewport size
    Vector2 size = window->getSize();
    glViewport(0, 0, int(size.x), int(size.y));

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Clear the screen
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // red background for testing
    glClear(GL_COLOR_BUFFER_BIT);
}

void RendererGL::endFrame() {
    window->swapGLBuffers();
}