#pragma once

#include <algorithm>

#include "cached_array.hpp"
#include "edge.hpp"
#include "index_keeper.hpp"
#include "vertex.hpp"

class graph {
public:
    graph(const size_t deg)
        : V(std::make_unique<vertex[]>(deg), deg), E(), v_loc(deg + 1) {
        for (size_t i = 1; i <= deg; ++i) {
            V[i].set_id(i);
            v_loc.add(i, i);
        }
    }

    graph(std::unique_ptr<vertex[]> p, const size_t size) : V(std::move(p), size) {
        assert(p.get() != nullptr);

		for (size_t i = 0; i < size; ++i) {
		}
    }

    constexpr size_t edge_degree() const noexcept { return E.degree(); }

    constexpr size_t degree() const noexcept { return V.degree(); }

    size_t degree(const size_t vertex_id) const noexcept {
        assert(vertex_id < V.degree());
        return V[vertex_id].degree();
    }

private:
    cached_array<vertex> V;
    cached_array<edge> E;
    index_keeper v_loc;
};
