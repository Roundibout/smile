#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>
#include <algorithm>

#include <datatypes/vector2.hpp>

#include <datatypes/object/point.hpp>
#include <datatypes/object/line.hpp>

#include <datatypes/object/vertex.hpp>
#include <datatypes/object/edge.hpp>

#include <core/instance.hpp>

constexpr float EPSILON = 1e-6f;

using Id = uint32_t;
constexpr Id INVALID_ID = UINT32_MAX;

class Object : public Instance {
public: // CHANGE AFTER TESTING
    // EXTERNAL

    // Points
    std::vector<Point> points;
    std::unordered_map<Id, size_t> pointIdToIndex;
    std::vector<Id> freePointIds;
    Id nextPointId = 0;

    // Lines
    std::vector<Line> lines;
    std::unordered_map<Id, size_t> lineIdToIndex;
    std::vector<Id> freeLineIds;
    Id nextLineId = 0;

    // INTERNAL

    // Vertices
    std::vector<Vertex> vertices;
    std::unordered_map<Id, size_t> vertexIdToIndex;
    Id nextVertexId = 0;

    // Edges
    std::vector<Edge> edges;
    std::unordered_map<Id, size_t> edgeIdToIndex;
    Id nextEdgeId = 0;

    Id hashLineIds(Id lineA, Id lineB) {
        if (lineA > lineB) std::swap(lineA, lineB); // Ensure smaller ID comes first to make it order independent
        return static_cast<Id>(lineA * 31 + lineB); // Simple deterministic combination
    }

    bool lineSegmentIntersection(const Vector2& p1, const Vector2& p2, const Vector2& q1, const Vector2& q2, Vector2& intersection) {
        // Represent lines as p + r*t and q + s*u
        Vector2 r = p2 - p1;
        Vector2 s = q2 - q1;

        float rxs = r.x * s.y - r.y * s.x;
        if (rxs == 0.0f) return false; // Lines are parallel or collinear

        Vector2 qp = q1 - p1;
        float t = (qp.x * s.y - qp.y * s.x) / rxs;
        float u = (qp.x * r.y - qp.y * r.x) / rxs;

        if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
            intersection = p1 + r * t;
            return true;
        }

        return false; // No intersection within the segments
    }
    
    bool isVertexOnLine(const Vertex& point, const Vertex& a, const Vertex& b) {
        // Vector cross product = 0 if collinear
        float cross = (point.y - a.y) * (b.x - a.x) - (point.x - a.x) * (b.y - a.y);
        if (std::fabs(cross) > EPSILON) return false;

        // Check if within bounding box
        float dot = (point.x - a.x) * (b.x - a.x) + (point.y - a.y) * (b.y - a.y);
        if (dot < 0) return false;

        float squaredLength = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
        if (dot > squaredLength) return false;

        return true;
    }

    void sortVerticesAlongLine(std::vector<Id>& verticesOnLine, const Vertex& a, const Vertex& b) {
        // Direction vector
        float dx = b.x - a.x;
        float dy = b.y - a.y;

        // Sort by projection t along AB
        std::sort(verticesOnLine.begin(), verticesOnLine.end(),
            [&](Id lhs, Id rhs) {
                const Vertex& v1 = vertexIdToIndex.at(lhs);
                const Vertex& v2 = vertexIdToIndex.at(rhs);

                float t1 = (dx != 0.0f) ? (v1.x - a.x) / dx : (v1.y - a.y) / dy;
                float t2 = (dx != 0.0f) ? (v2.x - a.x) / dx : (v2.y - a.y) / dy;
                return t1 < t2;
            });
    }
public:
    Object(std::string name) : Instance(name) {}

    // POINTS

    Id createPoint(const Vector2& position);
    void deletePoint(Id id);

    const Point* getPoint(Id id);

    // LINES

    Id createLine(Id point1, Id point2);
    void deleteLine(Id id);

    const Line* getLine(Id id);

    // INTERNAL

    void compute();
};