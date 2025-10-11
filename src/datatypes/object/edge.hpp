#pragma once

#include <cstdint>

using Id = uint32_t;

struct Edge {
    Id id;
    Id start, end;
    bool forwardUsed = false;
    bool backwardUsed = false;

    Edge(Id id, Id start, Id end) : id(id), start(start), end(end) {}
};