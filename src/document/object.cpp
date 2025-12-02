#include "document/object.hpp"

Point::Id Object::createPoint(const Vector2& position) {
    Point::Id id;

    if (not freePointIds.empty()) { // Are there any free points?
        id = freePointIds.back(); // Use it
        freePointIds.pop_back(); // Remove it from the list
    } else {
        id = nextPointId++; // Otherwise, use the next point id and increment it
    }

    points.push_back(std::make_unique<Point>(id, position)); // Add the point to the point vector

    pointIdToIndex[id] = points.size() - 1; // Link this id with the index of the point in the point vector

    solve();

    return id;
}

void Object::deletePoint(Point::Id id) {
    auto it = pointIdToIndex.find(id); // Get an iterator from the map
    if (it == pointIdToIndex.end()) return; // Invalid id

    size_t index = it->second; // Get the point's index
    std::swap(points[index], points.back()); // Swap the point we want to delete with the point at the front of the point vector
    pointIdToIndex[points[index]->id] = index; // Update moved point index that used to be at the front
    points.pop_back(); // Remove the point we want to delete that we just moved to the front

    pointIdToIndex.erase(it); // Unlink this id from the deleted point's index

    // Recycle id only if it wasn't the last one
    if (id + 1 == nextPointId) {
        --nextPointId; // Rewind next id
    } else {
        freePointIds.push_back(id); // Otherwise, add to free list
    }

    solve();
}

std::unique_ptr<Point>& Object::getPoint(Point::Id id) {
    auto it = pointIdToIndex.find(id); // Get an iterator from the map
    // TODO: What to do when the id doesn't exist?
    return points[it->second];
}

Line::Id Object::createLine(Point::Id point1, Point::Id point2) {
    if (point1 == point2) return INVALID_ID; // Points are the same
    if (pointIdToIndex.find(point1) == pointIdToIndex.end()) return INVALID_ID; // Invalid point1 id
    if (pointIdToIndex.find(point2) == pointIdToIndex.end()) return INVALID_ID; // Invalid point2 id

    Line::Id id;

    if (not freeLineIds.empty()) { // Are there any free lines?
        id = freeLineIds.back(); // Use it
        freeLineIds.pop_back(); // Remove it from the list
    } else {
        id = nextLineId++; // Otherwise, use the next line id and increment it
    }

    lines.push_back(std::make_unique<Line>(id, point1, point2)); // Add the line to the line vector

    lineIdToIndex[id] = lines.size() - 1; // Link this id with the index of the line in the line vector

    solve();

    return id;
}

void Object::deleteLine(Line::Id id) {
    auto it = lineIdToIndex.find(id); // Get an iterator from the map
    if (it == lineIdToIndex.end()) return; // Invalid id

    size_t index = it->second; // Get the line's index
    std::swap(lines[index], lines.back()); // Swap the line we want to delete with the line at the front of the line vector
    lineIdToIndex[lines[index]->id] = index; // Update moved line index that used to be at the front
    lines.pop_back(); // Remove the line we want to delete that we just moved to the front

    lineIdToIndex.erase(it); // Unlink this id from the deleted line's index

    // Recycle id only if it wasn't the last one
    if (id + 1 == nextLineId) {
        --nextLineId; // Rewind next id
    } else {
        freeLineIds.push_back(id); // Otherwise, add to free list
    }

    solve();
}

std::unique_ptr<Line>& Object::getLine(Line::Id id) {
    auto it = lineIdToIndex.find(id); // Get an iterator from the map
    // TODO: What to do when the id doesn't exist?
    return lines[it->second]; // Return a pointer
}

void Object::solve() {
    
}