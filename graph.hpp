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
    graph() noexcept
        : _deg(0),
          _v_cap(2),
          _V(std::make_unique<vertex[]>(2)),
          _e_deg(0),
          _e_cap(2),
          _E(std::make_unique<edge[]>(2)),
          _intg_v(true) {}

    // TODO: handle case when vertices and edges are not consistent, use the
    // integrate function
    graph(std::unique_ptr<vertex[]> vertices, std::unique_ptr<edge[]> edges,
          size_t degree, size_t listsize, bool integrate)
        : _deg(degree),
          _v_cap(degree),
          _V(std::move(vertices)),
          _e_deg(listsize),
          _e_cap(listsize),
          _E(std::move(edges)),
          _intg_v(integrate) {}

    // assumes that if certain vertices that are smaller than max vertex id
    // mentioned are not connected by any of the edges, than those are isolated
    // vertices
    graph(std::unique_ptr<edge[]> edges, size_t listsize, bool integrate = true)
        : _e_deg(listsize),
          _e_cap(listsize),
          _E(std::move(edges)),
          _intg_v(integrate) {
        // finding the max id
        size_t max_id = 0;
        for (size_t i = 0; i < _e_cap; ++i) {
            if (max_id < _E[i].dest) max_id = _E[i].dest;
            if (max_id < _E[i].src) max_id = _E[i].src;
        }
        _deg = max_id;

        // next greatest power of 2 will be the capacity
        // doing this here because it is going to be declared out of the
        // initialization list
        size_t lt = 2;
        while (lt < max_id) lt <<= 1;
        _v_cap = lt;
        _V     = std::make_unique<vertex[]>(_v_cap);

        if (!_intg_v) return;

        // vertices are kept in order of id in _V, unlike in vertex object
        for (size_t i = 0; i < _e_cap; ++i) {
            // storing information at both ends
            _V[_E[i].src].join(_E[i].dest, _E[i].weight);
            _V[_E[i].dest].join(_E[i].src, _E[i].weight);
        }
    }

    // TODO: Complete the functions to add and integrate
    // need to handle cases where the vertices/edges may refer to more vertices
    // than originally defined
    void add(std::unique_ptr<vertex[]>& vertices) {}

    void add(std::unique_ptr<edge[]>& edges) {}

    // make vertices and edges consistent
    void integrate() {
        size_t e_max_id = 0, v_max_id = 0;

        for (size_t i = 0; i < _v_cap; ++i)
            if (v_max_id < _V[i].id()) v_max_id = _V[i].id();

        for (size_t i = 0; 0 < _e_cap; ++i) {
            if (e_max_id < _E[i].dest) e_max_id = _E[i].dest;
            if (e_max_id < _E[i].src) e_max_id = _E[i].src;
        }
    }

    /* Getters */
    size_t degree() const noexcept { return _deg; }
    size_t edge_count() const noexcept { return _e_deg; }

private:
    size_t _deg, _v_cap;
    std::unique_ptr<vertex[]> _V;
    size_t _e_deg, _e_cap;
    std::unique_ptr<edge[]> _E;

    // bools for optimization
    bool _intg_v;

    void resize_v(const size_t size) {
        // resize should not delete existing info
        if (size <= _deg) return;

        // reallocation most thread safe
        auto v_temp = std::make_unique<vertex[]>(size);

        if constexpr (std::is_nothrow_move_assignable_v<
                          std::unique_ptr<vertex[]>>) {
            std::move(_V.get(), _V.get() + _v_cap, v_temp.get());
        } else {
            std::copy(_V.get(), _V.get() + _v_cap, v_temp.get());
        }

        _V     = std::move(v_temp);
        _v_cap = size;
    }

    void resize_e(const size_t size) {
        // resize should not delete existing info
        if (size <= _deg) return;

        // reallocation most thread safe
        auto temp = std::make_unique<edge[]>(size);

        if constexpr (std::is_nothrow_move_assignable_v<
                          std::unique_ptr<edge[]>>) {
            std::move(_E.get(), _E.get() + _e_cap, temp.get());
        } else {
            std::copy(_E.get(), _E.get() + _e_cap, temp.get());
        }

        _E     = std::move(temp);
        _e_cap = size;
    }
};
