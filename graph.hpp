#pragma once

#include <algorithm>

#include "cached_array.hpp"
#include "edge.hpp"
#include "index_keeper.hpp"
#include "vertex.hpp"

class graph {
public:
    /* Constructors */
    graph(const size_t deg) : V(deg), E(), v_loc(deg + 1) {
        for (size_t i = 1; i <= deg; ++i) {
            V[i].set_id(i);
            v_loc.add(i, i);
        }
    }

    graph(std::unique_ptr<vertex[]> p, const size_t size)
        : V(std::move(p), size), E(size), v_loc(size + 1) {
        for (size_t i = 0; i < size; ++i)
            for (size_t j = 0; j < V[i].degree(); ++j) {
                if (V[i][j].dest > V[i].id())
                    E.add({V[i].id(), V[i][j].dest, V[i][j].weight});
                v_loc.add(V[i].id(), i);
            }
    }

    graph(std::unique_ptr<edge[]> p, const size_t size) : E(std::move(p), size) {
        for (size_t i = 0; i < size; i++) {
            V[E[i].src].join(E[i].dest);
        }
    }

    /* Getters */
    constexpr size_t edge_degree() const noexcept { return E.degree(); }
    constexpr size_t degree() const noexcept { return V.degree(); }

    constexpr size_t degree(const size_t vertex_id) const noexcept {
        assert(vertex_id < V.degree());
        return V[vertex_id].degree();
    }

    // UNTESED
    template <typename Compare = bool(const edge&, const edge&)>
    void sort_edges(Compare compare = [](const edge& e1, const edge& e2) -> bool {
        return e1.src > e2.src;
    }) {
        E.sort(compare);
    }

    /* adding new elements */
    void join(const vertex& v) {
        V.add(v);
        int i = 0;
        while (i >= 0) E.add(v.next(i));
    }

    void join(const edge& e) {
        E.add(e);
        V[v_loc[e.src]].join(e);
        V[v_loc[e.dest]].join(e);
    }

private:
    cached_array<vertex> V;
    cached_array<edge> E;
    index_keeper v_loc;
};

// UNTESED
graph& operator+(graph& G, const edge& e) {
    G.join(e);
    return G;
}

// UNTESED
graph& operator+(graph& G, const vertex& v) {
    G.join(v);
    return G;
}
