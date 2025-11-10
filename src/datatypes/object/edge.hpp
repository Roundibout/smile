#pragma once

#include <cstdint>

struct Edge {
    using Id = uint32_t;

    Id id;
    Id start, end;
    bool forwardUsed = false;
    bool backwardUsed = false;

    Edge(Id id, Id start, Id end) : id(id), start(start), end(end) {}
};