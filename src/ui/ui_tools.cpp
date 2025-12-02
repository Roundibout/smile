#include "ui_tools.hpp"

bool ui_tools::is_point_over_rect(const Vector2& point, const AbsoluteLayout& layout) {
    return point.x >= layout.rect.position.x and point.y >= layout.rect.position.y and point.x <= layout.rect.position.x + layout.rect.size.x and point.y <= layout.rect.position.y + layout.rect.size.y;
}

bool ui_tools::is_point_over_rounded_rect(const Vector2& point, const AbsoluteLayout& layout) {
    float x = point.x;
    float y = point.y;
    float left = layout.rect.position.x;
    float bottom = layout.rect.position.y;
    float right = left + layout.rect.size.x;
    float top = bottom + layout.rect.size.y;

    if (x < left or x > right or y < bottom or y > top) return false;

    float rLB = layout.cornerLB;
    float rRB = layout.cornerRB;
    float rLT = layout.cornerLT;
    float rRT = layout.cornerRT;

    // bottom-left
    if (x < left + rLB and y < bottom + rLB) {
        float dx = x - (left + rLB);
        float dy = y - (bottom + rLB);
        return (dx * dx + dy * dy) <= rLB * rLB;
    }

    // bottom-right
    if (x > right - rRB and y < bottom + rRB) {
        float dx = x - (right - rRB);
        float dy = y - (bottom + rRB);
        return (dx * dx + dy * dy) <= rRB * rRB;
    }

    // top-left
    if (x < left + rLT and y > top - rLT) {
        float dx = x - (left + rLT);
        float dy = y - (top - rLT);
        return (dx * dx + dy * dy) <= rLT * rLT;
    }

    // top-right
    if (x > right - rRT and y > top - rRT) {
        float dx = x - (right - rRT);
        float dy = y - (top - rRT);
        return (dx * dx + dy * dy) <= rRT * rRT;
    }

    return true;
}

Vector2 ui_tools::clamp_point_within_rect(const Vector2& point, const AbsoluteLayout& layout) {
    return Vector2(std::clamp(point.x, layout.rect.position.x, layout.rect.position.x + layout.rect.size.x), std::clamp(point.y, layout.rect.position.y, layout.rect.position.y + layout.rect.size.y));
}

Vector2 ui_tools::mirror_point_across_rect(const Vector2& point, const AbsoluteLayout& layout) {
    Vector2 mirrored;
    if (point.x < layout.rect.position.x) {
        mirrored.x = layout.rect.position.x + layout.rect.size.x - 4;
    } else if (point.x > layout.rect.position.x + layout.rect.size.x) {
        mirrored.x = layout.rect.position.x + 4;
    } else {
        mirrored.x = point.x;
    }
    if (point.y < layout.rect.position.y) {
        mirrored.y = layout.rect.position.y + layout.rect.size.y - 4;
    } else if (point.y > layout.rect.position.y + layout.rect.size.y) {
        mirrored.y = layout.rect.position.y + 4;
    } else {
        mirrored.y = point.y;
    }
    return mirrored;
}