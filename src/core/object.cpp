#include "object.hpp"

Id Object::createPoint(const Vector2& position) {
    Id id;

    if (!freePointIds.empty()) { // Are there any free points?
        id = freePointIds.back(); // Use it
        freePointIds.pop_back(); // Remove it from the list
    } else {
        id = nextPointId++; // Otherwise, use the next point id and increment it
    }

    points.push_back(Point{id, position}); // Add the point to the point vector

    pointIdToIndex[id] = points.size() - 1; // Link this id with the index of the point in the point vector

    return id;
}

void Object::deletePoint(Id id) {
    auto it = pointIdToIndex.find(id); // Get an iterator from the map
    if (it == pointIdToIndex.end()) return; // Invalid id

    size_t index = it->second; // Get the point's index
    std::swap(points[index], points.back()); // Swap the point we want to delete with the point at the front of the point vector
    pointIdToIndex[points[index].id] = index; // Update moved point index that used to be at the front
    points.pop_back(); // Remove the point we want to delete that we just moved to the front

    pointIdToIndex.erase(it); // Unlink this id from the deleted point's index

    // Recycle id only if it wasn't the last one
    if (id + 1 == nextPointId) {
        --nextPointId; // Rewind next id
    } else {
        freePointIds.push_back(id); // Otherwise, add to free list
    }
}

const Point* Object::getPoint(Id id) {
    auto it = pointIdToIndex.find(id); // Get an iterator from the map
    if (it == pointIdToIndex.end()) return nullptr; // Invalid id
    return &points[it->second]; // Return a pointer
}

Id Object::createLine(Id point1, Id point2) {
    if (point1 == point2) return INVALID_ID; // Points are the same
    if (pointIdToIndex.find(point1) == pointIdToIndex.end()) return INVALID_ID; // Invalid point1 id
    if (pointIdToIndex.find(point2) == pointIdToIndex.end()) return INVALID_ID; // Invalid point2 id

    Id id;

    if (!freeLineIds.empty()) { // Are there any free lines?
        id = freeLineIds.back(); // Use it
        freeLineIds.pop_back(); // Remove it from the list
    } else {
        id = nextLineId++; // Otherwise, use the next line id and increment it
    }

    lines.push_back(Line{id, point1, point2}); // Add the line to the line vector

    lineIdToIndex[id] = lines.size() - 1; // Link this id with the index of the line in the line vector

    return id;
}

void Object::deleteLine(Id id) {
    auto it = lineIdToIndex.find(id); // Get an iterator from the map
    if (it == lineIdToIndex.end()) return; // Invalid id

    size_t index = it->second; // Get the line's index
    std::swap(lines[index], lines.back()); // Swap the line we want to delete with the line at the front of the line vector
    lineIdToIndex[lines[index].id] = index; // Update moved line index that used to be at the front
    lines.pop_back(); // Remove the line we want to delete that we just moved to the front

    lineIdToIndex.erase(it); // Unlink this id from the deleted line's index

    // Recycle id only if it wasn't the last one
    if (id + 1 == nextLineId) {
        --nextLineId; // Rewind next id
    } else {
        freeLineIds.push_back(id); // Otherwise, add to free list
    }
}

const Line* Object::getLine(Id id) {
    auto it = lineIdToIndex.find(id); // Get an iterator from the map
    if (it == lineIdToIndex.end()) return nullptr; // Invalid id
    return &lines[it->second]; // Return a pointer
}

void Object::compute() {
    // Step 1: Convert from non-planar to planar
    //     Turn intersections into virtual points and combine them with real points into vertices
    //     Split lines at intersections and combine with existing lines into edges

    // Step 2: Compute faces from edges
    //     Use some sort of algorithm to figure this out
    //     We also must have holes working inside faces (But these holes will just be other faces and not actual holes. They can be made to look like holes in their appearance.)

    // Step 3 (maybe): Triangulate
    //     Not important yet, we'll do it after.

    // Add existing real points to vertex vector
    for (const Point& point : points) {
        vertices.emplace_back(Vertex(point.id, point.x, point.y));
        vertexIdToIndex[point.id] = vertices.size() - 1;
        if (point.id >= nextVertexId) nextVertexId = point.id + 1;
    }

    // Create virtual points from line intersections
    for (size_t i = 0; i < lines.size(); ++i) { // For every line
        const Line& lineA = lines[i]; // Get the line from the loop
        const Point& pointA1 = points[pointIdToIndex[lineA.point1]];
        const Point& pointA2 = points[pointIdToIndex[lineA.point2]];

        // Get the line's vertices' positions
        const Vector2& A1 = Vector2(pointA1.x, pointA1.y);
        const Vector2& A2 = Vector2(pointA2.x, pointA2.y);

        for (size_t j = i + 1; j < lines.size(); ++j) { // For every line, go through every line
            const Line& lineB = lines[j]; // Get the other line from this loop
            const Point& pointB1 = points[pointIdToIndex[lineB.point1]];
            const Point& pointB2 = points[pointIdToIndex[lineB.point2]];

            // Get the other line's vertices' positions
            const Vector2& B1 = Vector2(pointB1.x, pointB1.y);
            const Vector2& B2 = Vector2(pointB2.x, pointB2.y);

            // Now that we have the positions of two lines' vertices, we can see if the lines intersect
            Vector2 intersection;
            if (lineSegmentIntersection(A1, A2, B1, B2, intersection)) { // Check for intersection
                auto isSamePoint = [&](const Point& p) {
                    return std::fabs(p.x - intersection.x) < 1e-6f && std::fabs(p.y - intersection.y) < 1e-6f;
                };
                if (isSamePoint(pointA1) || isSamePoint(pointA2) ||
                    isSamePoint(pointB1) || isSamePoint(pointB2)) {
                    continue; // Already represented as a real point
                }

                // Check if we've already added this virtual point
                bool exists = false;
                for (const auto& v : vertices) {
                    if (std::fabs(v.x - intersection.x) < 1e-6f &&
                        std::fabs(v.y - intersection.y) < 1e-6f) {
                        exists = true;
                        break;
                    }
                }
                if (exists) continue;


                // Create the vertex's id
                Id interId = nextVertexId++;

                vertices.emplace_back(Vertex(interId, intersection));
                vertexIdToIndex[interId] = vertices.size() - 1;
                if (interId >= nextVertexId) nextVertexId = interId + 1;
            }
        }
    }

    // Split lines into edges at intersections
    for (const Line& line : lines) { // For every line
        std::vector<Id> verticesOnLine = {line.point1, line.point2}; // Collect endpoints

        const Vertex& vertex1 = vertices[vertexIdToIndex[line.point1]];
        const Vertex& vertex2 = vertices[vertexIdToIndex[line.point2]];

        // Check all intersection vertices
        for (const Vertex& vertex : vertices) { // For every line, go through every vertex
            if (vertex.id != line.point1 && vertex.id != line.point2) { // Ensure both of these aren't one of this line's endpoints
                // If vertex lies on the line segment
                if (isVertexOnLine(vertex, vertex1, vertex2)) {
                    verticesOnLine.push_back(vertex.id);
                }
            }
        }

        // Sort vertices along the line for correct order
        sortVerticesAlongLine(verticesOnLine, vertex1, vertex2);

        // Create edges between consecutive vertices
        for (size_t i = 0; i < verticesOnLine.size() - 1; ++i) {
            Id v1 = verticesOnLine[i];
            Id v2 = verticesOnLine[i+1];
            edges.emplace_back(Edge(nextEdgeId++, v1, v2));
            edgeIdToIndex[edges.back().id] = edges.size() - 1;
        }
    }
}