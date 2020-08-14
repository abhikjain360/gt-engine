#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <type_traits>

#include "edge.hpp"

// aim: to make sure vertex_list needs no direct access to
// 		the member variables of the vertex class
// preferably access the degree and capacity once in start
// and let it handle the rest automatically
class vertex {
public:
    /* Constructors */
    vertex(const size_t id) noexcept
        : _id(id),
          _deg(0),
          _cap(2),
          _wgt(1),
          _E(std::make_unique<joiner[]>(_cap)),
          _d_deg(0),
          _d_cap(_cap),
          _D(std::make_unique<size_t[]>(_cap)) {}

    vertex(const size_t id, const float weight,
           const size_t capacity = 2) noexcept
        : _id(id),
          _deg(0),
          _cap(capacity > 1 ? capacity : 2),
          _wgt(weight),
          _E(std::make_unique<joiner[]>(_cap)),
          _d_deg(0),
          _d_cap(_cap),
          _D(std::make_unique<size_t[]>(_cap)) {}

    vertex(const size_t id, std::unique_ptr<joiner[]> joiner_list,
           const size_t size, const float weight = 1)
        : _id(id),
          _deg(size),
          _cap(size > 1 ? size : 2),
          _wgt(weight),
          _E(std::move(joiner_list)),
          _d_deg(0),
          _d_cap(_cap),
          _D(std::make_unique<size_t[]>(_cap)) {}

    /* Setters */
    void set_weight(const float weight) noexcept { this->_wgt = weight; }
    void set_id(const size_t id) noexcept { this->_id = id; }

    /* Getters */
    size_t id() const noexcept { return _id; }
    float weight() const noexcept { return _wgt; }
    size_t capacity() const noexcept { return _cap; }
    size_t degree() const noexcept { return _deg; }

    std::optional<edge> edge_at(const size_t dest) const noexcept {
        if (dest > _deg) return {};

        for (size_t i = 0; i < _cap; ++i) {
            if (_E[i].dest == dest) {
                return edge{_E[i].weight, _id, dest};
            }
        }

        return {};
    }

    /* Add edges */
    void join(const size_t dest, const float weight = 1) {
        // if no cache left, means degree = last index filled + 1
        //                                = index to be filled now

        // vertex list to handle the vertex on the
        // other side of the edge

        // do this if capacity id not enough
        // note that degree == capacity only if
        // no del_cache is left
        if (_deg == _cap) {
            size_t lt = 2;
            while (lt <= _cap) lt <<= 1;
            this->resize(lt);

            _E[_deg++] = {dest, weight};
            return;
        }

        // do this if we have some cache
        // degree < capacity if cache left
        if (_d_deg > 0) {
            _E[_D[--_d_deg]] = {dest, weight};
            ++_deg;
            return;
        }

        // capacity enough, no cache
        _E[_deg++] = {dest, weight};
    }

    /* Remove edges :
       returns false if no such vertex existed,
           else returns true if it does and is deleted
    */
    bool unjoin(const size_t dest) {
        // even if last element deleted and added to del cache,
        // no problem

        // increas del cache x2 if not enough
        if (_d_deg == _d_cap) {
            size_t lt = 2;
            while (lt < _d_deg) lt <<= 1;

            this->resize_d(lt);
            _d_cap = lt;
        }

        // TODO: handle for cases when cache already there;
        // 		 1. indexing if _Edges would not be same as _Deg
        // 		 2. optimize if deleting from last
        for (size_t i = 0; i < _cap; ++i) {
            if (_E[i].dest == dest) {
                _E[i]        = {0, 0};
                _D[_d_deg++] = i;
                --_deg;
                return true;
            }
        }

        return false;
    }

    // TODO: functions to add:
    // 		 2) make all connections unique (use sets)
    // 		 3) unjoin all connections with a certain vertex

private:
    // the very basics needed
    size_t _id, _deg, _cap;
    float _wgt;
    std::unique_ptr<joiner[]> _E;

    // del degree will always point at last valid index + 1
    // del cache stores only the 0 based indices
    // of free edges storage
    size_t _d_deg = 0, _d_cap;
    std::unique_ptr<size_t[]> _D; // stores indices

    /* Resize */
    bool resize(size_t size) {
        // resize should not delete existing info
        if (size <= _deg) return false;

        // reallocation most thread safe
        auto temp = std::make_unique<joiner[]>(size);

        if constexpr (std::is_nothrow_move_assignable_v<
                          std::unique_ptr<joiner[]>>) {
            std::move(_E.get(), _E.get() + _cap, temp.get());
        } else {
            std::copy(_E.get(), _E.get() + _cap, temp.get());
        }

        _E   = std::move(temp);
        _cap = size;

        return true;
    }

    bool resize_d(size_t size) {
        // resize should not delete existing info
        if (size <= _d_deg) return false;

        // reallocation most thread safe
        auto del_temp = std::make_unique<size_t[]>(size);

        if constexpr (std::is_nothrow_move_assignable_v<
                          std::unique_ptr<size_t[]>>) {
            std::move(_D.get(), _D.get() + _d_cap, del_temp.get());
        } else {
            std::copy(_D.get(), _D.get() + _d_cap, del_temp.get());
        }

        _D     = std::move(del_temp);
        _d_cap = size;

        return true;
    }
};
