/*
File:
    renderer.cpp
Authors:
    Lucas
Purpose:
    Implementation of the Renderer class
License:
    MIT (see LICENSE file)
*/

#include "renderer.hpp"

Renderer::Renderer(WindowImpl* window) {
    impl = std::make_unique<RendererGL>(window); // only OpenGL for now (a while probably)
}

Vector2 Renderer::resolvePosition(const UIDim2& position, const UIBounds& bounds, bool subpixel) {
    return impl->resolvePosition(position, bounds, subpixel);
}

AbsoluteLayout Renderer::resolveLayout(const UILayout& layout, const UIBounds& bounds, bool subpixel) {
    return impl->resolveLayout(layout, bounds, subpixel);
}

UIBounds Renderer::applyLayout(const UIBounds& bounds, const UILayout& layout) {
    return impl->applyLayout(bounds, layout);
}

void Renderer::beginFrame(float scale) {
    impl->beginFrame(scale);
}

void Renderer::drawTriangle(const UIDim2& position1, const UIDim2& position2, const UIDim2& position3, const UIBounds& bounds, const Color4& color) {
    impl->drawTriangle(position1, position2, position3, bounds, color);
}

void Renderer::drawQuad(const UIDim2& position1, const UIDim2& position2, const UIDim2& position3, const UIDim2& position4, const UIBounds& bounds, const Color4& color) {
    impl->drawQuad(position1, position2, position3, position4, bounds, color);
}

void Renderer::drawLine(const UIDim2& position1, const UIDim2& position2, const UIBounds& bounds, const Color4& color, float thickness) {
    float dx = position2.x.offset - position1.x.offset;
    float dy = position2.y.offset - position1.y.offset;
    float len = std::sqrt(dx*dx + dy*dy);

    if (len == 0) return; // Skip degenerate line

    dx /= len;
    dy /= len;

    float px = -dy;
    float py = dx;

    px *= thickness / 2;
    py *= thickness / 2;

    UIDim2 offset1(0.0f, px, 0.0f, py);
    UIDim2 offset2(0.0f, -px, 0.0f, -py);

    impl->drawQuad(
        position1 + offset1,
        position1 + offset2,
        position2 + offset2,
        position2 + offset1,
        bounds, color
    );
}

void Renderer::drawDottedLine(const UIDim2& position1, const UIDim2& position2, const UIBounds& bounds, const Color4& color, float thickness, float dotLength, float spacing) {
    float dx = position2.x.offset - position1.x.offset;
    float dy = position2.y.offset - position1.y.offset;
    float len = std::sqrt(dx*dx + dy*dy);

    if (len == 0) return; // Skip degenerate line

    float dirX = dx / len;
    float dirY = dy / len;

    float currentPos = 0.0f;

    while (currentPos < len) {
        float startOffset = currentPos;
        float endOffset = std::min(currentPos + dotLength, len);

        UIDim2 dotStart(
            position1.x.scale,
            position1.x.offset + dirX * startOffset,
            position1.y.scale,
            position1.y.offset + dirY * startOffset
        );

        UIDim2 dotEnd(
            position1.x.scale,
            position1.x.offset + dirX * endOffset,
            position1.y.scale,
            position1.y.offset + dirY * endOffset
        );

        // Draw the dot as a line
        drawLine(dotStart, dotEnd, bounds, color, thickness);

        // Advance to the next dot
        currentPos += dotLength + spacing;
    }
}

void Renderer::drawStrokeDottedLine(const UIDim2& position1, const UIDim2& position2, const UIBounds& bounds, const Color4& color, float thickness, float dotLength, float spacing, float stroke, const Color4& strokeColor) {
    float strokeSpacing = spacing - stroke * 2;

    Vector2 delta = resolvePosition(position2, bounds, true) - resolvePosition(position1, bounds, true);
    float angle = std::atan2(delta.x, -delta.y);

    float cos = std::cosf(angle);
    float sin = std::sinf(angle);
    Vector2 offset(
        -sin,
        cos
    );

    float trim = std::sqrtf(delta.x*delta.x + delta.y*delta.y); // TODO: use this to adjust the dotted stroke

    if (strokeSpacing <= 0) {
        // TODO: should use draw line instead
        drawDottedLine(
            UIDim2(position1.x.scale, position1.x.offset + offset.x * stroke, position1.y.scale, position1.y.offset + offset.y * stroke), 
            UIDim2(position2.x.scale, position2.x.offset - offset.x * stroke, position2.y.scale, position2.y.offset - offset.y * stroke), 
            bounds, strokeColor, thickness + stroke * 2, dotLength + stroke * 2, strokeSpacing);
    } else {
        drawDottedLine(
            UIDim2(position1.x.scale, position1.x.offset + offset.x * stroke, position1.y.scale, position1.y.offset + offset.y * stroke), 
            UIDim2(position2.x.scale, position2.x.offset - offset.x * stroke, position2.y.scale, position2.y.offset - offset.y * stroke), 
            bounds, strokeColor, thickness + stroke * 2, dotLength + stroke * 2, strokeSpacing);
    }

    drawDottedLine(position1, position2, bounds, color, thickness, dotLength, spacing);
}

void Renderer::drawRect(const UILayout& layout, const UIBounds& bounds, const Color4& color) {
    impl->drawRect(layout, bounds, color);
}

void Renderer::drawStrokeRect(const UILayout& layout, const UIBounds& bounds, const Color4& color, int stroke, const Color4& strokeColor, const UIStrokeAlignment& strokeAlignment) {
    impl->drawStrokeRect(layout, bounds, color, stroke, strokeColor, strokeAlignment);
}

void Renderer::drawRoundedRect(const UILayout& layout, const UIBounds& bounds, const Color4& color) {
    impl->drawRoundedRect(layout, bounds, color);
}

void Renderer::drawRoundedStrokeRect(const UILayout& layout, const UIBounds& bounds, const Color4& color, int stroke, const Color4& strokeColor, const UIStrokeAlignment& strokeAlignment) {
    impl->drawRoundedStrokeRect(layout, bounds, color, stroke, strokeColor, strokeAlignment);
}

void Renderer::drawArrow(const UIDim2& position, const UIBounds& bounds, float rotation, const Color4& color, float stumpLength, float armLength, float lineThickness) {
    std::vector<Vector2> points;
    
    points.push_back(Vector2(-lineThickness / 2.0f, 0.0f)); // stumpBL 0
    points.push_back(Vector2(lineThickness / 2.0f, 0.0f)); // stumpBR 1
    points.push_back(Vector2(-lineThickness / 2.0f, static_cast<float>(stumpLength))); // stumpTL 2
    points.push_back(Vector2(lineThickness / 2.0f, static_cast<float>(stumpLength))); // stumpTR 3

    points.push_back(Vector2(0.0f, stumpLength + lineThickness / 2)); // tip 4

    points.push_back(Vector2(points[4].x + armLength / ROOT2, points[4].y - armLength / ROOT2)); // armRBR 5
    points.push_back(Vector2(points[5].x - lineThickness / ROOT2, points[5].y - lineThickness / ROOT2)); // armRBL 6
    points.push_back(Vector2(points[6].x - armLength / ROOT2, points[6].y + armLength / ROOT2)); // armRTL 7

    points.push_back(Vector2(-points[5].x, points[5].y)); // armLBR 8
    points.push_back(Vector2(-points[6].x, points[6].y)); // armLBL 9
    points.push_back(Vector2(-points[7].x, points[7].y)); // armLTL 10

    float cosR = std::cosf(rotation);
    float sinR = std::sinf(rotation);

    for (Vector2& point : points) {
        point = Vector2(
            (point.x * cosR - point.y * sinR),
            (point.x * sinR + point.y * cosR)
        );
    }

    impl->drawQuad(
        UIDim2(position.x.scale, position.x.offset + points[0].x, position.y.scale, position.y.offset + points[0].y),
        UIDim2(position.x.scale, position.x.offset + points[1].x, position.y.scale, position.y.offset + points[1].y),
        UIDim2(position.x.scale, position.x.offset + points[3].x, position.y.scale, position.y.offset + points[3].y),
        UIDim2(position.x.scale, position.x.offset + points[2].x, position.y.scale, position.y.offset + points[2].y),
        bounds,
        color
    );

    impl->drawQuad(
        UIDim2(position.x.scale, position.x.offset + points[4].x, position.y.scale, position.y.offset + points[4].y),
        UIDim2(position.x.scale, position.x.offset + points[7].x, position.y.scale, position.y.offset + points[7].y),
        UIDim2(position.x.scale, position.x.offset + points[6].x, position.y.scale, position.y.offset + points[6].y),
        UIDim2(position.x.scale, position.x.offset + points[5].x, position.y.scale, position.y.offset + points[5].y),
        bounds,
        color
    );

    impl->drawQuad(
        UIDim2(position.x.scale, position.x.offset + points[4].x, position.y.scale, position.y.offset + points[4].y),
        UIDim2(position.x.scale, position.x.offset + points[8].x, position.y.scale, position.y.offset + points[8].y),
        UIDim2(position.x.scale, position.x.offset + points[9].x, position.y.scale, position.y.offset + points[9].y),
        UIDim2(position.x.scale, position.x.offset + points[10].x, position.y.scale, position.y.offset + points[10].y),
        bounds,
        color
    );
}

void Renderer::drawStrokeArrow(const UIDim2& position, const UIBounds& bounds, float rotation, const Color4& color, float stumpLength, float armLength, float lineThickness, float stroke, const Color4& strokeColor) {
    std::vector<Vector2> points;
    
    points.push_back(Vector2(-lineThickness / 2.0f - stroke, static_cast<float>(-stroke))); // stumpBL 0
    points.push_back(Vector2(lineThickness / 2.0f + stroke, static_cast<float>(-stroke))); // stumpBR 1
    points.push_back(Vector2(-lineThickness / 2.0f - stroke, static_cast<float>(stumpLength))); // stumpTL 2
    points.push_back(Vector2(lineThickness / 2.0f + stroke, static_cast<float>(stumpLength))); // stumpTR 3

    points.push_back(Vector2(0.0f, stumpLength + lineThickness / 2 + stroke * ROOT2)); // tip 4

    points.push_back(Vector2(points[4].x + armLength / ROOT2 + stroke * ROOT2, points[4].y - armLength / ROOT2 - stroke * ROOT2)); // armRBR 5
    points.push_back(Vector2(points[5].x - lineThickness / ROOT2 - stroke * ROOT2, points[5].y - lineThickness / ROOT2 - stroke * ROOT2)); // armRBL 6
    points.push_back(Vector2(points[6].x - armLength / ROOT2 - stroke * ROOT2, points[6].y + armLength / ROOT2 + stroke * ROOT2)); // armRTL 7

    points.push_back(Vector2(-points[5].x, points[5].y)); // armLBR 8
    points.push_back(Vector2(-points[6].x, points[6].y)); // armLBL 9
    points.push_back(Vector2(-points[7].x, points[7].y)); // armLTL 10

    float cosR = std::cosf(rotation);
    float sinR = std::sinf(rotation);

    for (Vector2& point : points) {
        point = Vector2(
            (point.x * cosR - point.y * sinR),
            (point.x * sinR + point.y * cosR)
        );
    }

    impl->drawQuad(
        UIDim2(position.x.scale, position.x.offset + points[0].x, position.y.scale, position.y.offset + points[0].y),
        UIDim2(position.x.scale, position.x.offset + points[1].x, position.y.scale, position.y.offset + points[1].y),
        UIDim2(position.x.scale, position.x.offset + points[3].x, position.y.scale, position.y.offset + points[3].y),
        UIDim2(position.x.scale, position.x.offset + points[2].x, position.y.scale, position.y.offset + points[2].y),
        bounds,
        strokeColor
    );

    impl->drawQuad(
        UIDim2(position.x.scale, position.x.offset + points[4].x, position.y.scale, position.y.offset + points[4].y),
        UIDim2(position.x.scale, position.x.offset + points[7].x, position.y.scale, position.y.offset + points[7].y),
        UIDim2(position.x.scale, position.x.offset + points[6].x, position.y.scale, position.y.offset + points[6].y),
        UIDim2(position.x.scale, position.x.offset + points[5].x, position.y.scale, position.y.offset + points[5].y),
        bounds,
        strokeColor
    );

    impl->drawQuad(
        UIDim2(position.x.scale, position.x.offset + points[4].x, position.y.scale, position.y.offset + points[4].y),
        UIDim2(position.x.scale, position.x.offset + points[8].x, position.y.scale, position.y.offset + points[8].y),
        UIDim2(position.x.scale, position.x.offset + points[9].x, position.y.scale, position.y.offset + points[9].y),
        UIDim2(position.x.scale, position.x.offset + points[10].x, position.y.scale, position.y.offset + points[10].y),
        bounds,
        strokeColor
    );

    drawArrow(position, bounds, rotation, color, stumpLength, armLength, lineThickness);
}

void Renderer::drawSolidArrow(const UIDim2& position, const UIBounds& bounds, float rotation, const Color4& color, const Vector2& stumpSize, const Vector2& headSize) {
    std::vector<Vector2> points;

    points.push_back(Vector2(-stumpSize.x / 2, 0.0f));
    points.push_back(Vector2(stumpSize.x / 2, 0.0f));
    points.push_back(Vector2(-stumpSize.x / 2, stumpSize.y));
    points.push_back(Vector2(stumpSize.x / 2, stumpSize.y));

    points.push_back(Vector2(-headSize.x / 2, stumpSize.y));
    points.push_back(Vector2(headSize.x / 2, stumpSize.y));
    points.push_back(Vector2(0.0f, stumpSize.y + headSize.y));

    float cosR = std::cosf(rotation);
    float sinR = std::sinf(rotation);

    for (Vector2& point : points) {
        point = Vector2(
            (point.x * cosR - point.y * sinR),
            (point.x * sinR + point.y * cosR)
        );
    }

    impl->drawQuad(
        UIDim2(position.x.scale, position.x.offset + points[0].x, position.y.scale, position.y.offset + points[0].y),
        UIDim2(position.x.scale, position.x.offset + points[1].x, position.y.scale, position.y.offset + points[1].y),
        UIDim2(position.x.scale, position.x.offset + points[3].x, position.y.scale, position.y.offset + points[3].y),
        UIDim2(position.x.scale, position.x.offset + points[2].x, position.y.scale, position.y.offset + points[2].y),
        bounds,
        color
    );

    impl->drawTriangle(
        UIDim2(position.x.scale, position.x.offset + points[4].x, position.y.scale, position.y.offset + points[4].y),
        UIDim2(position.x.scale, position.x.offset + points[5].x, position.y.scale, position.y.offset + points[5].y),
        UIDim2(position.x.scale, position.x.offset + points[6].x, position.y.scale, position.y.offset + points[6].y),
        bounds,
        color
    );
}

static Vector2 normalizeVec(const Vector2& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    return (len > 0.00001f) ? Vector2(v.x / len, v.y / len) : Vector2(0, 0);
}

static Vector2 outwardNormal(const Vector2& p1, const Vector2& p2) {
    Vector2 d = normalizeVec(p2 - p1);
    return Vector2(d.y, -d.x);
}

static Vector2 lineIntersection(const Vector2& p1, const Vector2& n1, const Vector2& p2, const Vector2& n2) {
    Vector2 d1(-n1.y, n1.x);
    Vector2 d2(-n2.y, n2.x);
    float denom = d1.x * d2.y - d1.y * d2.x;
    if (std::fabs(denom) < 1e-6f)
        return p1;
    float t = ((p2.x - p1.x) * d2.y - (p2.y - p1.y) * d2.x) / denom;
    return p1 + d1 * t;
}

void Renderer::drawSolidStrokeArrow(const UIDim2& position, const UIBounds& bounds, float rotation, const Color4& color, const Vector2& stumpSize, const Vector2& headSize, float stroke, const Color4& strokeColor) {
    std::vector<Vector2> points;

    points.push_back(Vector2(-stumpSize.x / 2 - stroke, static_cast<float>(-stroke)));
    points.push_back(Vector2(stumpSize.x / 2 + stroke, static_cast<float>(-stroke)));
    points.push_back(Vector2(-stumpSize.x / 2 - stroke, stumpSize.y + stroke));
    points.push_back(Vector2(stumpSize.x / 2 + stroke, stumpSize.y + stroke));
    
    // triangle (head)
    Vector2 A(-headSize.x / 2, stumpSize.y);
    Vector2 B(headSize.x / 2, stumpSize.y);
    Vector2 C(0.0f, stumpSize.y + headSize.y);

    Vector2 nAB = outwardNormal(A, B);
    Vector2 nBC = outwardNormal(B, C);
    Vector2 nCA = outwardNormal(C, A);

    Vector2 A_off1 = A + nAB * stroke;
    Vector2 B_off1 = B + nAB * stroke;
    Vector2 B_off2 = B + nBC * stroke;
    Vector2 C_off2 = C + nBC * stroke;
    Vector2 C_off3 = C + nCA * stroke;
    Vector2 A_off3 = A + nCA * stroke;

    points.push_back(lineIntersection(A_off3, nCA, A_off1, nAB));
    points.push_back(lineIntersection(B_off1, nAB, B_off2, nBC));
    points.push_back(lineIntersection(C_off2, nBC, C_off3, nCA));

    float cosR = std::cosf(rotation);
    float sinR = std::sinf(rotation);

    for (Vector2& point : points) {
        point = Vector2(
            (point.x * cosR - point.y * sinR),
            (point.x * sinR + point.y * cosR)
        );
    }

    impl->drawQuad(
        UIDim2(position.x.scale, position.x.offset + points[0].x, position.y.scale, position.y.offset + points[0].y),
        UIDim2(position.x.scale, position.x.offset + points[1].x, position.y.scale, position.y.offset + points[1].y),
        UIDim2(position.x.scale, position.x.offset + points[3].x, position.y.scale, position.y.offset + points[3].y),
        UIDim2(position.x.scale, position.x.offset + points[2].x, position.y.scale, position.y.offset + points[2].y),
        bounds,
        strokeColor
    );

    impl->drawTriangle(
        UIDim2(position.x.scale, position.x.offset + points[4].x, position.y.scale, position.y.offset + points[4].y),
        UIDim2(position.x.scale, position.x.offset + points[5].x, position.y.scale, position.y.offset + points[5].y),
        UIDim2(position.x.scale, position.x.offset + points[6].x, position.y.scale, position.y.offset + points[6].y),
        bounds,
        strokeColor
    );

    drawSolidArrow(position, bounds, rotation, color, stumpSize, headSize);
}

void Renderer::drawText(const UIDim2& position, const UIBounds& bounds, const std::string& text, const std::string& path, int size, const Color4& color, bool shadow) {
    if (shadow) {
        impl->drawText(position - UIDim2(0.0f, 0, 0.0f, 2), bounds, text, path, size, Color4(0.0f, 0.0f, 0.0f, color.a * 0.75));
    }
    impl->drawText(position, bounds, text, path, size, color);
}

void Renderer::enableSubpixel() {
    impl->enableSubpixel();
}

void Renderer::disableSubpixel() {
    impl->disableSubpixel();
}

void Renderer::beginStencil() {
    impl->beginStencil();
}

void Renderer::useStencil() {
    impl->useStencil();
}

void Renderer::endStencil() {
    impl->endStencil();
}

void Renderer::endFrame() {
    impl->endFrame();
}

void Renderer::dirty() {
    dirtied = true;
}

void Renderer::clean() {
    dirtied = false;
}

bool Renderer::isDirty() {
    return dirtied;
}