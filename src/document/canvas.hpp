#pragma once

#include "type/rect.hpp"
#include "type/color4.hpp"

class Canvas {
private:
    Rect rect;
    Color4 color;
public:
    Canvas(const Rect& rect) : rect(rect) {}

    Rect getSize() {return rect;}
    Color4 getColor() {return color;}
};