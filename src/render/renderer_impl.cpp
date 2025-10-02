/*
File:
    renderer_impl.cpp
Authors:
    Lucas
Purpose:
    Implementation of the Renderer implementation class
License:
    MIT (see LICENSE file)
*/

#include "renderer_impl.hpp"

RendererImpl::RendererImpl(WindowImpl* w) {
    window = w;
}

Vector2 RendererImpl::resolvePosition(const UIDim2& position, const UIBounds& bounds) {
    UIBounds applied = applyLayout(bounds, UILayout(UIRect(position, UIDim2())));

    return Vector2(
        static_cast<int>(applied.absolute.x * applied.layout.rect.position.x.scale + applied.layout.rect.position.x.offset * currentScale),
        static_cast<int>(applied.absolute.y * applied.layout.rect.position.y.scale + applied.layout.rect.position.y.offset * currentScale)
    );
}

AbsoluteLayout RendererImpl::resolveLayout(const UILayout& layout, const UIBounds& bounds) {
    UIBounds applied = applyLayout(bounds, layout);

    AbsoluteLayout resolved(
        Rect(
            applied.absolute.x * applied.layout.rect.position.x.scale + applied.layout.rect.position.x.offset * currentScale,
            applied.absolute.y * applied.layout.rect.position.y.scale + applied.layout.rect.position.y.offset * currentScale,
            applied.absolute.x * applied.layout.rect.size.x.scale + applied.layout.rect.size.x.offset * currentScale,
            applied.absolute.y * applied.layout.rect.size.y.scale + applied.layout.rect.size.y.offset * currentScale
        )
    );

    resolved.cornerRT = applied.layout.cornerRT.offset * currentScale;
    resolved.cornerLT = applied.layout.cornerLT.offset * currentScale;
    resolved.cornerRB = applied.layout.cornerRB.offset * currentScale;
    resolved.cornerLB = applied.layout.cornerLB.offset * currentScale;

    float sizeX = resolved.rect.size.x;
    float sizeY = resolved.rect.size.y;
    float halfX = sizeX / 2.0f;
    float halfY = sizeY / 2.0f;

    if (sizeX >= sizeY) {
        resolved.cornerRT = std::clamp(resolved.cornerRT + halfY * applied.layout.cornerRT.scale, 0.0f, halfY);
        resolved.cornerLT = std::clamp(resolved.cornerLT + halfY * applied.layout.cornerLT.scale, 0.0f, halfY);
        resolved.cornerRB = std::clamp(resolved.cornerRB + halfY * applied.layout.cornerRB.scale, 0.0f, halfY);
        resolved.cornerLB = std::clamp(resolved.cornerLB + halfY * applied.layout.cornerLB.scale, 0.0f, halfY);
    } else {
        resolved.cornerRT = std::clamp(resolved.cornerRT + halfX * applied.layout.cornerRT.scale, 0.0f, halfX);
        resolved.cornerLT = std::clamp(resolved.cornerLT + halfX * applied.layout.cornerLT.scale, 0.0f, halfX);
        resolved.cornerRB = std::clamp(resolved.cornerRB + halfX * applied.layout.cornerRB.scale, 0.0f, halfX);
        resolved.cornerLB = std::clamp(resolved.cornerLB + halfX * applied.layout.cornerLB.scale, 0.0f, halfX);
    }

    return resolved;
}

UIBounds RendererImpl::applyLayout(const UIBounds& bounds, const UILayout& childLayout) {
    UIBounds applied(bounds.absolute);

    applied.layout.rect.position.x.scale = bounds.layout.rect.position.x.scale + (bounds.layout.rect.size.x.scale * childLayout.rect.position.x.scale);
    applied.layout.rect.position.x.offset = bounds.layout.rect.position.x.offset + childLayout.rect.position.x.offset + (bounds.layout.rect.size.x.offset * childLayout.rect.position.x.scale);

    applied.layout.rect.position.y.scale = bounds.layout.rect.position.y.scale + (bounds.layout.rect.size.y.scale * childLayout.rect.position.y.scale);
    applied.layout.rect.position.y.offset = bounds.layout.rect.position.y.offset + childLayout.rect.position.y.offset + (bounds.layout.rect.size.y.offset * childLayout.rect.position.y.scale);

    applied.layout.rect.size.x.scale = bounds.layout.rect.size.x.scale * childLayout.rect.size.x.scale;
    applied.layout.rect.size.x.offset = (bounds.layout.rect.size.x.offset * childLayout.rect.size.x.scale) + childLayout.rect.size.x.offset;

    applied.layout.rect.size.y.scale = bounds.layout.rect.size.y.scale * childLayout.rect.size.y.scale;
    applied.layout.rect.size.y.offset = (bounds.layout.rect.size.y.offset * childLayout.rect.size.y.scale) + childLayout.rect.size.y.offset;

    applied.layout.cornerRT = childLayout.cornerRT;
    applied.layout.cornerLT = childLayout.cornerLT;
    applied.layout.cornerRB = childLayout.cornerRB;
    applied.layout.cornerLB = childLayout.cornerLB;

    return applied;
}