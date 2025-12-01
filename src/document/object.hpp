#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

import numbers;
#include "type/vector2.hpp"
#include "type/point.hpp"
#include "type/line.hpp"
#include "type/shape.hpp"

#include "document/instance.hpp"
#include "util/console.hpp"

enum class EdgeDirection {
    Forward,
    Backward
};

class Object : public Instance {
public:
    using Id = u32;
    inline static constexpr u32 INVALID_ID = UINT32_MAX;
private:
    // Points
    std::vector<std::unique_ptr<Point>> points;
    std::unordered_map<Point::Id, size_t> pointIdToIndex;
    std::vector<Point::Id> freePointIds;
    Point::Id nextPointId = 0;

    // Lines
    std::vector<std::unique_ptr<Line>> lines;
    std::unordered_map<Line::Id, size_t> lineIdToIndex;
    std::vector<Line::Id> freeLineIds;
    Line::Id nextLineId = 0;

    // Shapes
    std::vector<std::unique_ptr<Shape>> shapes;
    std::unordered_map<Shape::Id, size_t> shapeIdToIndex;
    std::vector<Shape::Id> freeShapeIds;
    Shape::Id nextShapeId = 0;
public:
    Object(std::string name) : Instance(name) {}

    // POINTS

    Point::Id createPoint(const Vector2& position);
    void deletePoint(Point::Id id);

    std::unique_ptr<Point>& getPoint(Point::Id id);
    std::vector<std::unique_ptr<Point>>& getPoints() {return points;}

    // LINES

    Line::Id createLine(Point::Id point1, Point::Id point2);
    void deleteLine(Line::Id id);

    std::unique_ptr<Line>& getLine(Line::Id id);
    std::vector<std::unique_ptr<Line>>& getLines() {return lines;}

    // This function needs to be done BETTER!
    // Should actually work correctly and be optimized to solve only parts affected by changes
    void solve();
};