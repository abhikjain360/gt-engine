#pragma once

#include <cstddef>

// used internally, inside vertex to point to other vertices
struct joiner {
	size_t dest;
    float weight;
};

// to be used by the user, and in graph class
struct edge {
	float weight;
	size_t src, dest;
};
