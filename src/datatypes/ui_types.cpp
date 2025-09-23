#include "ui_types.hpp"

Rect UILayout::resolve(const UIBounds& bounds) const {
    UIBounds applied = bounds.applyLayout(*this);
    return Rect(
        applied.absolute.x * applied.layout.rect.position.x.scale + applied.layout.rect.position.x.offset,
        applied.absolute.y * applied.layout.rect.position.y.scale + applied.layout.rect.position.y.offset,
        applied.absolute.x * applied.layout.rect.size.x.scale + applied.layout.rect.size.x.offset,
        applied.absolute.y * applied.layout.rect.size.y.scale + applied.layout.rect.size.y.offset
    );
}

UIBounds UIBounds::applyLayout(const UILayout& childLayout) const {
    UIBounds applied(absolute);

    applied.layout.rect.position.x.scale = layout.rect.position.x.scale + (layout.rect.size.x.scale * childLayout.rect.position.x.scale);
    applied.layout.rect.position.x.offset = layout.rect.position.x.offset + childLayout.rect.position.x.offset + (layout.rect.size.x.offset * childLayout.rect.position.x.scale);

    applied.layout.rect.position.y.scale = layout.rect.position.y.scale + (layout.rect.size.y.scale * childLayout.rect.position.y.scale);
    applied.layout.rect.position.y.offset = layout.rect.position.y.offset + childLayout.rect.position.y.offset + (layout.rect.size.y.offset * childLayout.rect.position.y.scale);

    applied.layout.rect.size.x.scale = layout.rect.size.x.scale * childLayout.rect.size.x.scale;
    applied.layout.rect.size.x.offset = layout.rect.size.x.offset + childLayout.rect.size.x.offset - (layout.rect.size.x.offset * childLayout.rect.size.x.scale);

    applied.layout.rect.size.y.scale = layout.rect.size.y.scale * childLayout.rect.size.y.scale;
    applied.layout.rect.size.y.offset = layout.rect.size.y.offset + childLayout.rect.size.y.offset - (layout.rect.size.y.offset * childLayout.rect.size.y.scale);

    return applied;
}