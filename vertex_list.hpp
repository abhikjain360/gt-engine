#pragma once

#include "vertex.hpp"

struct edge {
	float weight;
	size_t src, dest;
};


// Note: vertex indexing starts at 1
