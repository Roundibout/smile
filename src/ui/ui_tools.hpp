#pragma once

#include <algorithm>

#include "type/vector2.hpp"
#include "type/rect.hpp"
#include "type/ui_types.hpp"

namespace ui_tools {
    bool is_point_over_rect(const Vector2& point, const AbsoluteLayout& layout);
    bool is_point_over_rounded_rect(const Vector2& point, const AbsoluteLayout& layout);
    Vector2 clamp_point_within_rect(const Vector2& point, const AbsoluteLayout& layout);
    Vector2 mirror_point_across_rect(const Vector2& point, const AbsoluteLayout& layout);
}