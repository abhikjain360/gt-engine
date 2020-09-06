#pragma once

#include <algorithm>
#include <memory>

#include "edge.hpp"
#include "vertex.hpp"

// TODO: make serperate cpp file for the implementation

// Note: vertex indexing starts at 1
// vertex_list handles adding info about edge on both vertices
// vertex_list also to handle edge struct operations
class graph {
public:
    // UNTESTED
    graph() noexcept
        : _deg(0),
          _v_cap(2),
          _V(std::make_unique<vertex[]>(2)),
          _e_deg(0),
          _e_cap(2),
          _E(std::make_unique<edge[]>(2)),
          _intg_v(true),
          _vd_deg(0),
          _vd_cap(2),
          _Vd(std::make_unique<size_t[]>(2)),
          _ed_deg(0),
          _ed_cap(2),
          _Ed(std::make_unique<size_t[]>(2)) {}

    // UNTESTED
    graph(const size_t degree, const size_t listsize) noexcept
        : _deg(degree),
          _v_cap(_deg),
          _V(std::make_unique<vertex[]>(_v_cap)),
          _e_deg(listsize),
          _e_cap(_e_deg),
          _E(std::make_unique<edge[]>(_e_cap)),
          _intg_v(false),
          _vd_deg(0),
          _vd_cap(degree),
          _Vd(std::make_unique<size_t[]>(degree)),
          _ed_deg(0),
          _ed_cap(degree),
          _Ed(std::make_unique<size_t[]>(degree)) {}

    // UNTESTED
    graph(std::unique_ptr<vertex[]> vertices, std::unique_ptr<edge[]> edges,
          const size_t degree, const size_t listsize, const bool integrate = true)
        : _deg(degree),
          _v_cap(degree),
          _V(std::move(vertices)),
          _e_deg(listsize),
          _e_cap(listsize),
          _E(std::move(edges)),
          _intg_v(integrate),
          _vd_deg(0),
          _vd_cap(2),
          _Vd(std::make_unique<size_t[]>(2)),
          _ed_deg(0),
          _ed_cap(2),
          _Ed(std::make_unique<size_t[]>(2)) {
        if (_intg_v) this->integrate();
    }

    // assumes that if certain vertices that are smaller than max vertex id
    // mentioned are not connected by any of the edges, than those are isolated
    // vertices
    // TODO: integrate with deleted cache
    // UNTESTED
    graph(std::unique_ptr<edge[]> edges, const size_t listsize,
          const bool integrate = true)
        : _e_deg(listsize),
          _e_cap(listsize),
          _E(std::move(edges)),
          _intg_v(integrate),
          _vd_deg(0),
          _vd_cap(2),
          _Vd(std::make_unique<size_t[]>(2)),
          _ed_deg(0),
          _ed_cap(2),
          _Ed(std::make_unique<size_t[]>(2)) {
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

        // vertices are kept in order of id in _V when constructing this
        for (size_t i = 0; i < _e_cap; ++i) {
            // storing information at both ends
            _V[_E[i].src].join(_E[i].dest, _E[i].weight);
            _V[_E[i].dest].join(_E[i].src, _E[i].weight);
        }
    }

    /* Getters */
    // UNTESTED, but shold work fine
    size_t degree() const noexcept { return _deg; }
    size_t edge_count() const noexcept { return _e_deg; }

    // TODO: Complete the functions to add, remove and integrate
    // need to handle cases where the vertices/edges may refer to more vertices
    // than originally defined
    // we have different functions to add single and multiple values in order to
    // optimize for the multiple values ones using sorting
    // UNTESTED
    void add(const vertex& e) {
        if (_deg == _v_cap) {
            size_t lt = 2;
            while (lt <= _v_cap) lt <<= 1;
            this->resize_v(lt);
        }
    }
    // UNTESTED
    void add(std::unique_ptr<vertex[]>& vertices, const size_t size) {}

    // UNTESTED
    void add(const edge& e) {}
    // UNTESTED
    void add(std::unique_ptr<edge[]>& edges, const size_t size) {}

    // UNTESTED
    void remove(const vertex& e) {}
    // UNTESTED
    void remove(std::unique_ptr<vertex[]>& vertices, const size_t size) {}

    // UNTESTED
    void remove(const edge& e) {}
    // UNTESTED
    void remove(std::unique_ptr<edge[]>& edges, const size_t size) {}

    // return a copy
    // UNTESTED
    graph copy() const {}

    // make vertices and edges consistent
    // beacuse of this, we need to make sure that degree is always correct,
    // atleast upto the point user defines
    // UNTESTED
    void integrate() {
        size_t max_id = _v_cap, lt;

        for (size_t i = 0; i < _e_cap; ++i) {
            if (_E[i].src > max_id || _E[i].dest > max_id) {
                if (_E[i].src > _E[i].dest) {
                    lt = 2;
                    while (lt <= _E[i].src) lt <<= 1;
                    this->resize_v(lt);
                } else {
                    lt = 2;
                    while (lt <= _E[i].dest) lt <<= 1;
                    this->resize_v(lt);
                }

                _V[_E[i].src].join(_E[i].dest, _E[i].weight);
                _V[_E[i].dest].join(_E[i].src, _E[i].weight);
            }
        }

        _deg = max_id;
    }

private:
    size_t _deg, _v_cap;
    std::unique_ptr<vertex[]> _V;

    size_t _e_deg, _e_cap;
    std::unique_ptr<edge[]> _E;

    // bools for optimization
    bool _intg_v;

    size_t _vd_deg, _vd_cap;
    std::unique_ptr<size_t[]> _Vd;

    size_t _ed_deg, _ed_cap;
    std::unique_ptr<size_t[]> _Ed;

    // resize only affects the capacity, not the degree
    // UNTESTED
    void resize_v(const size_t size) {
        // resize should not delete existing info
        if (size <= _v_cap) return;

        // reallocation most thread safe
        auto v_temp = std::make_unique<vertex[]>(size);

        if constexpr (std::is_nothrow_move_assignable_v<std::unique_ptr<vertex[]>>) {
            std::move(_V.get(), _V.get() + _v_cap, v_temp.get());
        } else {
            std::copy(_V.get(), _V.get() + _v_cap, v_temp.get());
        }

        _V     = std::move(v_temp);
        _v_cap = size;
    }

    // resize only affects the capacity, not the degree
    // UNTESTED
    void resize_e(const size_t size) {
        // resize should not delete existing info
        if (size <= _e_cap) return;

        // reallocation most thread safe
        auto temp = std::make_unique<edge[]>(size);

        if constexpr (std::is_nothrow_move_assignable_v<std::unique_ptr<edge[]>>) {
            std::move(_E.get(), _E.get() + _e_cap, temp.get());
        } else {
            std::copy(_E.get(), _E.get() + _e_cap, temp.get());
        }

        _E     = std::move(temp);
        _e_cap = size;
    }
};

// TODO: these operations are to return a copy of the graph
// UNTESTED
graph operator+(const graph& G, const edge& e) {}

// UNTESTED
graph operator-(const graph& G, const edge& e) {}
