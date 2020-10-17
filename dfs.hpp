#pragma once

#include "graph.hpp"

void
dfs(const graph& G, const size_t vertex_id)
{
    assert(G.degree() >= vertex_id);

    auto arr = new bool[G.degree()];
    arr      = { 0 };

    arr[vertex_id] = 1;
}
