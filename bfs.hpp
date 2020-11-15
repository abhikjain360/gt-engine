#pragma once

#include "graph.hpp"
#include <queue>
#include <memory>

std::shared_ptr<uint8_t[]>
bfs(const graph& G, const size_t vertex_id)
{
    assert(G.degree() >= vertex_id);

    // arr index = vertex id - 1
    auto arr = std::make_shared<uint8_t[]>(G.degree());
    arr      = { 0 };

    std::queue<size_t> q;

    return arr;
}
