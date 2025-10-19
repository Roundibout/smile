#pragma once

#include <algorithm>

#include <datatypes/vector2.hpp>
#include <datatypes/rect.hpp>
#include <datatypes/ui_types.hpp>

namespace UITools {
    bool isPointOverRect(const Vector2& point, const AbsoluteLayout& layout);
    bool isPointOverRoundedRect(const Vector2& point, const AbsoluteLayout& layout);
    Vector2 clampPointWithinRect(const Vector2& point, const AbsoluteLayout& layout);
    Vector2 mirrorPointAcrossRect(const Vector2& point, const AbsoluteLayout& layout);
}