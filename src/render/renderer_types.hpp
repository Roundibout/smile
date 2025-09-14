/*
File:
    renderer_types.hpp
Authors:
    Lucas
Purpose:
    Definitions of enums and structures used with the Renderer class
License:
    MIT (see LICENSE file)
*/

#pragma once

#include <datatypes/vector2.hpp>
#include <datatypes/color4.hpp>

enum class RenderBackend {
    GL
};

struct Vertex {
    Vector2 position;
    Color4 color;
};