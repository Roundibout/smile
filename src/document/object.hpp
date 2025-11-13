#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <stack>
#include <memory>

#include <datatypes/vector2.hpp>

#include <datatypes/object/point.hpp>
#include <datatypes/object/line.hpp>
#include <datatypes/object/shape.hpp>

#include <datatypes/object/vertex.hpp>
#include <datatypes/object/edge.hpp>
#include <datatypes/object/face.hpp>

#include <document/instance.hpp>

#include <util/logger.hpp>

constexpr float EPSILON = 1e-6f;
constexpr float PI = 3.1415926f;

enum class EdgeDirection {
    Forward,
    Backward
};

constexpr uint32_t INVALID_ID = UINT32_MAX;

class Object : public Instance {
public:
    using Id = uint32_t;
public: // CHANGE AFTER TESTING
    // EXTERNAL

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

    // INTERNAL

    // Vertices
    std::vector<Vertex> vertices;
    Vertex::Id nextVertexId = 0;

    // Edges
    std::vector<Edge> edges;
    Edge::Id nextEdgeId = 0;

    // Faces
    std::vector<Face> faces;
    Face::Id nextFaceId = 0;

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
            [&](Vertex::Id lhs, Vertex::Id rhs) {
                const Vertex& v1 = vertices[lhs];
                const Vertex& v2 = vertices[rhs];

                float t1, t2;
                if (std::fabs(dx) > 1e-6f) {
                    t1 = (v1.x - a.x) / dx;
                    t2 = (v2.x - a.x) / dx;
                } else if (std::fabs(dy) > 1e-6f) {
                    t1 = (v1.y - a.y) / dy;
                    t2 = (v2.y - a.y) / dy;
                } else {
                    t1 = t2 = 0.0f; // degenerate line
                }

                return t1 < t2;
            });

        // Remove duplicate vertices if any
        verticesOnLine.erase(
            std::unique(verticesOnLine.begin(), verticesOnLine.end()),
            verticesOnLine.end()
        );
    }

    float ccwAngleFromEdges(float px, float py, float ax, float ay, float bx, float by) {
        float vax = ax - px;
        float vay = ay - py;

        float vbx = bx - px;
        float vby = by - py;

        // Compute cross and dot
        float cross = vax * vby - vay * vbx;
        float dot = vax * vbx + vay * vby;

        // atan2 gives signed angle in (-pi, pi], positive is CCW
        float angle = atan2f(cross, dot) * 180.0f / PI;

        if (angle < 0.0f) angle += 360.0f; // normalize to [0, 360)

        return angle;
    }

    Vertex::Id traverseEdge(Edge& startEdge, EdgeDirection direction) { 
        Vertex::Id aId;
        Vertex::Id pivotId;

        // Return invalid id if we have already traversed this way and set this edge's A and pivot vertex ids depending on direction
        if (direction == EdgeDirection::Forward) {
            if (startEdge.forwardUsed) {
                //Logger::warn("Forward already used");
                return INVALID_ID;
            }
            aId = startEdge.start;
            pivotId = startEdge.end;

            startEdge.forwardUsed = true; // We're checking forward
        } else if (direction == EdgeDirection::Backward) {
            if (startEdge.backwardUsed) {
                //Logger::warn("Backward already used");
                return INVALID_ID;
            }
            aId = startEdge.end;
            pivotId = startEdge.start;

            startEdge.backwardUsed = true; // We're checking backward
        }

        std::vector<Edge::Id> edgesConnectedByStart;
        std::vector<Edge::Id> edgesConnectedByEnd;

        // Find edges connected to the pivot
        for (const Edge& edge : edges) {
            if (edge.id != startEdge.id) {
                if (edge.start == pivotId && !edge.forwardUsed) {
                    edgesConnectedByStart.push_back(edge.id);
                } else if (edge.end == pivotId && !edge.backwardUsed) {
                    edgesConnectedByEnd.push_back(edge.id);
                }
            }
        }

        // Get A vertex and pivot vertex
        const Vertex& a = vertices[aId];
        const Vertex& pivot = vertices[pivotId];

        // Find lowest angle
        Edge::Id lowestAngleId = INVALID_ID;
        float lowestAngle = 360.0f;

        for (Edge::Id edgeId : edgesConnectedByStart) {
            Edge& edge = edges[edgeId];

            const Vertex& b = vertices[edge.end];

            float angle = ccwAngleFromEdges(pivot.x, pivot.y, a.x, a.y, b.x, b.y);

            if (angle < lowestAngle) {
                lowestAngleId = edgeId;
                lowestAngle = angle;
            }
        }
        for (Edge::Id edgeId : edgesConnectedByEnd) {
            Edge& edge = edges[edgeId];

            const Vertex& b = vertices[edge.start];

            float angle = ccwAngleFromEdges(pivot.x, pivot.y, a.x, a.y, b.x, b.y);

            if (angle < lowestAngle) {
                lowestAngleId = edgeId;
                lowestAngle = angle;
            }
        }

        if (lowestAngleId != INVALID_ID) {
            //Logger::print("Edge " + std::to_string(startEdge.id) + " Connection");
            //Logger::print("    " + std::to_string(lowestAngleId) + " at angle " + std::to_string(lowestAngle));
        } else {
            //Logger::error("Could not find a next edge");
        }

        return lowestAngleId;
    }

    void computeFace(Edge& startEdge, EdgeDirection startDirection) {
        Face face(nextFaceId++);
        //Logger::print("Computing face " + std::to_string(face.id) + " (edge " + std::to_string(startEdge.id));
        if (startDirection == EdgeDirection::Forward) {
            //Logger::append(" forward");
        } else {
            //Logger::append(" backward");
        }
        //Logger::append(")");
            
        face.edges.push_back(startEdge.id);

        bool traversing = true;
        bool completed = false;
        Edge::Id currentEdgeId = startEdge.id;
        EdgeDirection direction = startDirection;

        Vertex::Id start;
        if (direction == EdgeDirection::Forward) {
            start = startEdge.start;
        } else {
            start = startEdge.end;
        }
        face.vertices.push_back(start);

        while (traversing) {
            Edge& currentEdge = edges[currentEdgeId];
            Edge::Id nextEdgeId = traverseEdge(currentEdge, direction);

            if (nextEdgeId == INVALID_ID) { // Failed
                traversing = false;
                break;
            }

            face.edges.push_back(nextEdgeId);

            const Edge& nextEdge = edges[nextEdgeId];
            Vertex::Id pivot;
            if (direction == EdgeDirection::Forward) {
                pivot = currentEdge.end;
            } else {
                pivot = currentEdge.start;
            }

            if (nextEdge.start == pivot) { // Forward
                   direction = EdgeDirection::Forward;

                   face.vertices.push_back(nextEdge.start);

                if (nextEdge.end == start) { // Completed forward loop
                    completed = true;
                    traversing = false;
                    break;
                }
            } else { // Backward
                direction = EdgeDirection::Backward;

                face.vertices.push_back(nextEdge.end);

                if (nextEdge.start == start) { // Completed forward loop (using backward edge)
                    completed = true;
                    traversing = false;
                    break;
                }
            }

            currentEdgeId = nextEdgeId;
        };

        if (completed) {
            //Logger::print("FACE " + std::to_string(face.id));
            for (Edge::Id edge : face.edges) {
                //Logger::append("\n    " + std::to_string(edge));
            }

            faces.push_back(face); // Add face if it is completed
        } else {
            --nextFaceId;
        }

        //Logger::append("\n");
    }

    bool isCCW(const Face& face) {
        float sum = 0.0f;
        for (size_t i = 0; i < face.vertices.size(); i++) {
            const Vertex& v0 = vertices[face.vertices[i]];
            const Vertex& v1 = vertices[face.vertices[(i + 1) % face.vertices.size()]];
            sum += (v1.x - v0.x) * (v1.y + v0.y);
        }
        return sum < 0.0f;
    }

    float cross(const Vertex& a, const Vertex& b, const Vertex& c) {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

    bool pointInTriangle(const Vertex& p, const Vertex& a, const Vertex& b, const Vertex& c) {
        float c1 = cross(a, b, p);
        float c2 = cross(b, c, p);
        float c3 = cross(c, a, p);
        bool hasNeg = (c1 < 0) || (c2 < 0) || (c3 < 0);
        bool hasPos = (c1 > 0) || (c2 > 0) || (c3 > 0);
        return !(hasNeg && hasPos); // all same sign => inside
    }
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

    // INTERNAL

    void compute();
};