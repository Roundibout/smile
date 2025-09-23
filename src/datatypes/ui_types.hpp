#pragma once

#include <cmath>
#include <algorithm>

#include <datatypes/vector2.hpp>
#include <datatypes/rect.hpp>

struct UIDim {
    float scale;
    int offset;

    UIDim() : scale(0.0f), offset(0) {}
    UIDim(float scale, int offset) : scale(scale), offset(offset) {}

    bool operator==(const UIDim& other) const {
        return scale == other.scale && offset == other.offset;
    }

    bool operator!=(const UIDim& other) const {
        return scale != other.scale || offset != other.offset;
    }
};

struct UIDim2 {
    UIDim x, y;

    UIDim2() : x(UIDim()), y(UIDim()) {}
    UIDim2(const UIDim& x, const UIDim& y) : x(x), y(y) {}
    UIDim2(float scaleX, int offsetX, float scaleY, int offsetY) : x(UIDim(scaleX, offsetX)), y(UIDim(scaleY, offsetY)) {}

    bool operator==(const UIDim2& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const UIDim2& other) const {
        return x != other.x || y != other.y;
    }
};

struct UIRect {
    UIDim2 position, size;
    
    UIRect() : position(UIDim2()), size(UIDim2()) {}
    UIRect(const UIDim2& position, const UIDim2& size) : position(position), size(size) {}

    bool operator==(const UIRect& other) const {
        return position == other.position && size == other.size;
    }

    bool operator!=(const UIRect& other) const {
        return position != other.position || size != other.size;
    }
};

struct UIBounds; // Forward declaration

struct UILayout {
    UIRect rect;

    UILayout() : rect(UIRect()) {}
    UILayout(const UIRect& rect) : rect(rect) {}

    Rect resolve(const UIBounds& bounds) const;
};

struct UIBounds {
    Vector2 absolute;
    UILayout layout;

    UIBounds() : absolute(Vector2()), layout(UILayout()) {}
    UIBounds(const Vector2& absolute) : absolute(absolute), layout(UILayout()) {}
    UIBounds(const Vector2& absolute, const UILayout& layout) : absolute(absolute), layout(layout) {}

    UIBounds applyLayout(const UILayout& childLayout) const;
};