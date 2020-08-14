#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <type_traits>

// used internally, inside vertex to point to other vertices
struct joiner {
    size_t dest;
    float weight;
};

// aim: to make sure vertex_list needs no direct access to
// 		the member variables of the vertex class
// preferably access the degree and capacity once in start
// and let it handle the rest automatically
class vertex {
public:
    /* Constructors */
	vertex()
        : _Deg(0),
          _Cap(2),
          _Wgt(1),
          _Edges(std::make_unique<joiner[]>(_Cap)),
          _del_Deg(0),
          _del_Cap(_Cap),
          _del_Cache(std::make_unique<size_t[]>(_Cap)) {}

    vertex(float weight, size_t capacity = 2)
        : _Deg(0),
          _Cap(capacity > 1 ? capacity : 2),
          _Wgt(weight),
          _Edges(std::make_unique<joiner[]>(_Cap)),
          _del_Deg(0),
          _del_Cap(_Cap),
          _del_Cache(std::make_unique<size_t[]>(_Cap)) {}

    vertex(std::unique_ptr<joiner[]> joiner_list, size_t size,
           float weight = 1)
        : _Deg(size),
          _Cap(size > 1 ? size : 2),
          _Wgt(weight),
          _Edges(std::move(joiner_list)),
		  _del_Deg(0),
          _del_Cap(_Cap),
          _del_Cache(std::make_unique<size_t[]>(_Cap)) {}

    /* Setters */
    void set_weight(float weight) noexcept { this->_Wgt = weight; }

    /* Getters */
    float weight() const noexcept { return _Wgt; }
    size_t capacity() const noexcept { return _Cap; }
    size_t degree() const noexcept { return _Deg; }

    /* Add edges */
    void join(size_t dest, float weight = 1) {
        // if no cache left, means degree = last index filled + 1
        //                                = index to be filled now

        // vertex list to handle the vertex on the
        // other side of the edge

        // do this if capacity id not enough
        // note that degree == capacity only if
        // no del_cache is left
        if (_Deg == _Cap) {
            size_t lt = 2;
            while (lt <= _Cap) lt <<= 1;
            this->resize(lt);

            _Edges[_Deg++] = {dest, weight};
            return;
        }

        // do this if we have some cache
        // degree < capacity if cache left
        if (_del_Deg > 0) {
            _Edges[_del_Cache[--_del_Deg]] = {dest, weight};
            ++_Deg;
            return;
        }

        // capacity enough, no cache
        _Edges[_Deg++] = {dest, weight};
    }

    /* Remove edges :
       returns false if no such vertex existed,
           else returns true if it does and is deleted
    */
    bool unjoin(size_t dest) {
        // even if last element deleted and added to del cache,
        // no problem

        // increas del cache x2 if not enough
        if (_del_Deg == _del_Cap) {
            size_t lt = 2;
            while (lt < _del_Deg) lt <<= 1;

            this->resize_del_cache(lt);
            _del_Cap = lt;
        }

        // TODO: handle for cases when cache already there;
        // 		 1. indexing if _Edges would not be same as _Deg
        // 		 2. optimize if deleting from last
        for (size_t i = 0; i < _Cap; ++i) {
            if (_Edges[i].dest == dest) {
                _Edges[i]              = {0, 0};
                _del_Cache[_del_Deg++] = i;
                --_Deg;
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
    size_t _Deg, _Cap;
	float _Wgt;
    std::unique_ptr<joiner[]> _Edges;

    // del degree will always point at last valid index + 1
    // del cache stores only the 0 based indices
    // of free edges storage
    size_t _del_Deg = 0, _del_Cap;
    std::unique_ptr<size_t[]> _del_Cache; // stores indices

    /* Resize */
    bool resize(size_t size) {
        // resize should not delete existing info
        if (size <= _Deg) return false;

        // reallocation most thread safe
        auto temp = std::make_unique<joiner[]>(size);

        if constexpr (std::is_nothrow_move_assignable_v<
                          std::unique_ptr<joiner[]>>) {
            std::move(_Edges.get(), _Edges.get() + _Cap, temp.get());
        } else {
            std::copy(_Edges.get(), _Edges.get() + _Cap, temp.get());
        }

        _Edges = std::move(temp);
        _Cap   = size;

        return true;
    }

    bool resize_del_cache(size_t size) {
        // resize should not delete existing info
        if (size <= _del_Deg) return false;

        // reallocation most thread safe
        auto del_temp = std::make_unique<size_t[]>(size);

        if constexpr (std::is_nothrow_move_assignable_v<
                          std::unique_ptr<size_t[]>>) {
            std::move(_del_Cache.get(), _del_Cache.get() + _del_Cap,
                      del_temp.get());
        } else {
            std::copy(_del_Cache.get(), _del_Cache.get() + _del_Cap,
                      del_temp.get());
        }

        _del_Cache = std::move(del_temp);
        _del_Cap   = size;

        return true;
    }
};
