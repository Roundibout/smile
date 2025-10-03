#pragma once

#include <cmath>
#include <algorithm>
#include <string>

#include <datatypes/vector2.hpp>
#include <datatypes/rect.hpp>

struct UIDim {
    float scale;
    float offset;

    UIDim() : scale(0.0f), offset(0) {}
    UIDim(float scale, int offset) : scale(scale), offset(static_cast<float>(offset)) {}
    UIDim(float scale, float offset) : scale(scale), offset(offset) {}

    UIDim operator+(const UIDim& other) const {
        return UIDim(scale + other.scale, offset + other.offset);
    }

    UIDim operator-(const UIDim& other) const {
        return UIDim(scale - other.scale, offset - other.offset);
    }

    bool operator==(const UIDim& other) const {
        return scale == other.scale && offset == other.offset;
    }

    bool operator!=(const UIDim& other) const {
        return scale != other.scale || offset != other.offset;
    }

    UIDim operator+=(const UIDim& other) {
        scale += other.scale;
        offset += other.offset;
        return *this;
    }

    UIDim operator-=(const UIDim& other) {
        scale -= other.scale;
        offset -= other.offset;
        return *this;
    }

    std::string to_string() const;
};

struct UIDim2 {
    UIDim x, y;

    UIDim2() : x(UIDim()), y(UIDim()) {}
    UIDim2(const UIDim& x, const UIDim& y) : x(x), y(y) {}
    UIDim2(float scaleX, int offsetX, float scaleY, int offsetY) : x(UIDim(scaleX, offsetX)), y(UIDim(scaleY, offsetY)) {}
    UIDim2(float scaleX, float offsetX, float scaleY, float offsetY) : x(UIDim(scaleX, offsetX)), y(UIDim(scaleY, offsetY)) {}

    UIDim2 operator+(const UIDim2& other) const {
        return UIDim2(x + other.x, y + other.y);
    }

    UIDim2 operator-(const UIDim2& other) const {
        return UIDim2(x - other.x, y - other.y);
    }

    bool operator==(const UIDim2& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const UIDim2& other) const {
        return x != other.x || y != other.y;
    }

    UIDim2 operator+=(const UIDim2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    UIDim2 operator-=(const UIDim2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    std::string to_string() const;
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

struct AbsoluteLayout {
    Rect rect;

    float cornerRT, cornerLT, cornerRB, cornerLB = 0.0f;

    AbsoluteLayout() : rect(Rect()) {}
    AbsoluteLayout(const Rect& rect) : rect(rect) {}
};

struct UILayout {
    UIRect rect;

    UIDim cornerRT{};
    UIDim cornerLT{};
    UIDim cornerRB{};
    UIDim cornerLB{};

    UILayout() : rect(UIRect()) {}
    UILayout(const UIRect& rect) : rect(rect) {}

    void setCorners(const UIDim& corner) {
        cornerRT = corner;
        cornerLT = corner;
        cornerRB = corner;
        cornerLB = corner;
    }
};

struct UIBounds {
    Vector2 absolute;
    UILayout layout;

    UIBounds() : absolute(Vector2()), layout(UILayout()) {}
    UIBounds(const Vector2& absolute) : absolute(absolute), layout(UILayout()) {}
    UIBounds(const Vector2& absolute, const UILayout& layout) : absolute(absolute), layout(layout) {}
};

enum class UIStrokeAlignment {
    Outside = 0,
    Middle = 1,
    Inside = 2
};