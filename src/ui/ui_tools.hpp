#pragma once

#include <datatypes/vector2.hpp>
#include <datatypes/rect.hpp>
#include <datatypes/ui_types.hpp>

namespace UITools {
    bool isPointOverRect(const Vector2& point, const UIRect& rect, const UIRect& parentRect);
}