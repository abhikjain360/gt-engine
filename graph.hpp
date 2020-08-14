#pragma once

#include <algorithm>
#include <memory>

#include "edge.hpp"
#include "vertex.hpp"

// Note: vertex indexing starts at 1
// vertex_list handles adding info about edge on both vertices
// vertex_list also to handle edge struct operations
class graph {
public:
    graph()
        : _deg(0),
          _v_cap(2),
          _V(std::make_unique<vertex[]>(2)),
          _e_cap(2),
          _E(std::make_unique<edge[]>(2)) {}

    graph(std::unique_ptr<vertex[]> vertices, std::unique_ptr<edge[]> edges,
          size_t degree, size_t listsize)
        : _deg(degree),
          _v_cap(degree),
          _V(std::move(vertices)),
          _e_cap(listsize),
          _E(std::move(edges)) {}

    graph(std::unique_ptr<edge[]> edges, size_t listsize)
        : _deg(0),
          _v_cap(listsize * 2),
          _V(std::make_unique<vertex[]>(_v_cap)),
          _e_cap(listsize),
          _E(std::move(edges)) {
        std::sort(
            _E.get(), _E.get() + listsize,
            [](const edge& e1, const edge& e2) { return e1.dest <= e2.dest; });

        size_t temp = 0;

        for (size_t i = 0; i < listsize; ++i) {
            temp = _E[i].dest;

            do {

            } while (i < listsize && _E[i].dest == temp);

            ++_deg;
        }
    }

private:
    size_t _deg, _v_cap;
    std::unique_ptr<vertex[]> _V;
    size_t _e_cap;
    std::unique_ptr<edge[]> _E;

    void resize_v(size_t id) {}
};
