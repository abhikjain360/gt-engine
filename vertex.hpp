#pragma once

#include <algorithm>
#include <memory>
#include <type_traits>

// used internally, inside vertex to point to other vertices
struct joiner {
    float weight;
    size_t dest;
};

class vertex {
public:
    /* Construtors */
    // UNTESTED
    vertex(size_t capacity = 2, size_t weight = 1)
        : _Deg(0),
          _Wgt(weight),
          _Cap(capacity),
          _Edges(std::make_unique<joiner[]>(capacity)) {}

    // UNTESTED
    vertex(std::unique_ptr<joiner[]> joiner_list, size_t size,
           size_t weight = 1)
        : _Deg(size),
          _Wgt(weight),
          _Cap(size),
          _Edges(std::move(joiner_list)) {}

    /* Setters */
    // UNTESTED, but should work fine
    void set_weight(size_t weight) noexcept { this->_Wgt = weight; }

    /* Getters */
    // UNTESTED, but should work fine
    size_t weight() const noexcept { return _Wgt; }
    size_t capacity() const noexcept { return _Cap; }
    size_t degree() const noexcept { return _Deg; }

    /* Add edges */
    // UNTESTED
    void join(size_t dest, float weight) {
        // if no cache left, means degree = last index filled + 1
        //                                = index to be filled now

        // do this if capacity id not enough
        if (_Deg == _Cap) {
            size_t lt = 2;
            while (lt <= _Cap) lt <<= 1;
            this->resize(lt);

            _Edges[_Deg++] = {weight, dest};
            return;
        }

        // do this if we have some cache
        // degree < capacity if cache left
        if (_del_Deg > 0) {
            _Edges[_del_Cache[--_del_Deg]] = {weight, dest};
            ++_Deg;
            return;
        }

        // capacity enough, no cache
        _Edges[_Deg++] = {weight, dest};
    }

    /* Remove edges :
       returns false if no such vertex existed,
           else returns true if it does and is deleted
    */
    // UNTESTED
    bool unjoin(size_t dest) {
		if (_del_Deg == _del_Cap) {
			size_t lt = 2;
			while (lt < _del_Deg) lt <<= 1;

			this->resize_del_cache(lt);
			_del_Cap = lt;
		}

		bool deleted = false;

		// TODO: handle for cases when cache already there;
		// 		 indexing if _Edges would not be same as _Deg
		for (size_t i = 0; i < _Deg; ++i) {
			if (_Edges[i].dest == dest) {
				_Edges[i] = {0, 0};
				_del_Cache[_del_Deg++] = i;
			}
		}

		return deleted;
    }

	// TODO: functions to add:
	// 		 1) get number of connections to a certain vertex
	// 		 2) make all connections unique (use sets)
	// 		 3) unjoin all connections with a certain vertex

private:
    size_t _Deg, _Wgt, _Cap;
    std::unique_ptr<joiner[]> _Edges;
    size_t _del_Deg = 0, _del_Cap;
    std::unique_ptr<size_t[]> _del_Cache; // stores indices

    /* Resize */
    // UNTESTED
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

    // UNTESTED
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
