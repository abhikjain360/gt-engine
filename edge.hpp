#pragma once

#include <cstddef>

// used internally, inside vertex to point to other vertices
struct joiner {
    size_t dest;
    float weight;

	joiner() : dest(0), weight(0) {}
    joiner(size_t d, float w = 1) : dest(d), weight(w) {}
    explicit joiner(size_t d) : dest(d), weight(1) {}
};

// to be used by the user, and in graph class
struct edge {
    float weight;
    size_t src, dest;
};
