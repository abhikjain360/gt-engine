#pragma once

#include "vertex.hpp"

struct edge {
	float weight;
	size_t src, dest;
};


// Note: vertex indexing starts at 1
// vertex_list handles adding info about edge on both vertices
// vertex_list also to handle edge struct operations
