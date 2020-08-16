#pragma once

#include <cstddef>
#include <memory>

template <typename T>
class cached_array {
public:
    constexpr cached_array() noexcept
        : _deg(0),
          _cap(2),
          _ptr(std::make_unique<T[]>(2)),
          _d_deg(0),
          _d_cap(2),
          _d_ptr(std::make_unique<size_t[]>(2)) {}

    cached_array(std::unique_ptr<T[]> p, const size_t size)
        : _deg(size),
          _cap(next2(size)),
          _ptr(std::move(p)),
          _d_deg(0),
          _d_cap(_cap),
          _d_ptr(std::make_unique<size_t[]>(_d_cap)) {}

    void add(const T& t) {
        // if no cache left, means degree = last index filled + 1
        //                                = index to be filled now

        // vertex list to handle the vertex on the
        // other side of the edge

        // do this if capacity id not enough
        // note that degree == capacity only if
        // no del_cache is left
        if (_deg == _cap) {
            this->resize(_deg + 1);
            _ptr[_deg++] = t;
            return;
        }

        // do this if we have some cache
        // degree < capacity if cache left
        if (_d_deg > 0) {
            _ptr[_d_ptr[--_d_deg]] = t;
            ++_deg;
            return;
        }

        // capacity enough, no cache
        _ptr[_deg++] = t;
    }

    void add(std::unique_ptr<T>&& p, size_t size) {
        size_t lt = _deg + size - _d_deg;
        if (lt >= _cap) this->resize(lt);

        while (_d_deg > 0) {
        }
    }

    template <typename Compare>
    bool remove(const T& t, Compare compare) {
        // even if last element deleted and added to del cache,
        // no problem

        // increas del cache if not enough
        if (_d_deg == _d_cap) {
            this->resize_d(_d_deg + 1);
        }

        // TODO: handle for cases when cache already there;
        // 		 1. indexing if _Edges would not be same as _Deg
        // 		 2. optimize if deleting from last
        for (size_t i = 0; i < _cap; ++i) {
            if (compare(_d_ptr[i], t)) {
                _d_ptr[i]        = (T)0;
                _d_ptr[_d_deg++] = i;
                --_deg;
                return true;
            }
        }

        return false;
    }

    cached_array<T>& operator=(cached_array<T>&& arr) {
        // setting values of the LHS
        _deg = arr._deg;
        _cap = arr._cap;
        _ptr = std::move(arr._ptr);

        _d_deg = arr._d_deg;
        _d_cap = arr._d_cap;
        _d_ptr = std::move(arr._d_ptr);

        // removing values of the RHS
        arr._deg   = 0;
        arr._cap   = 0;
        arr._d_deg = 0;
        arr._d_cap = 0;

        return *this;
    }

    cached_array<T>& operator=(const cached_array<T>& arr) {
        _deg = arr._deg;
        _cap = arr._cap;
        for (size_t i = 0; i < _cap; ++i) _ptr[i] = arr._ptr[i];

        _d_deg = arr._d_deg;
        _d_cap = arr._d_cap;
        for (size_t i = 0; i < _d_cap; ++i) _d_ptr[i] = arr._d_ptr[i];

        return *this;
    }

    constexpr size_t degree() const noexcept { return _deg; }
    constexpr size_t capacity() const noexcept { return _cap; }

private:
    size_t _deg, _cap;
    std::unique_ptr<T[]> _ptr;

    size_t _d_deg, _d_cap;
    std::unique_ptr<size_t[]> _d_ptr;

    // utility functon
    constexpr size_t next2(size_t size) const noexcept {
        size_t lt = 2;
        while (lt <= size) lt <<= 1;
        return lt;
    }

    // Resize only affects the capacity, not the degree. Increases capacity to
    // next power of 2
    bool resize(const size_t size) {
        // resize should not delete existing info
        if (size <= _cap) return false;

        size_t lt = next2(size);

        // reallocation most thread safe
        auto temp = std::make_unique<T[]>(lt);

        if constexpr (std::is_nothrow_move_assignable_v<std::unique_ptr<T[]>>) {
            std::move(_ptr.get(), _ptr.get() + _cap, temp.get());
        } else {
            std::copy(_ptr.get(), _ptr.get() + _cap, temp.get());
        }

        _ptr = std::move(temp);
        _cap = lt;

        return true;
    }

    // Resize only affects the capacity, not the degree. Increases capacity to
    // next power of 2
    bool resize_d(const size_t size) {
        // resize should not delete existing info
        if (size <= _d_cap) return false;

        size_t lt = next2(size);

        // reallocation most thread safe
        auto temp = std::make_unique<size_t[]>(lt);

        if constexpr (std::is_nothrow_move_assignable_v<std::unique_ptr<size_t[]>>) {
            std::move(_d_ptr.get(), _d_ptr.get() + _d_cap, temp.get());
        } else {
            std::copy(_d_ptr.get(), _d_ptr.get() + _d_cap, temp.get());
        }

        _d_ptr = std::move(temp);
        _d_cap = lt;

        return true;
    }
};
