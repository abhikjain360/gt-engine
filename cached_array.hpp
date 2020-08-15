#pragma once

#include <cstddef>
#include <memory>

template <typename T>
class cached_array {
public:
	size_t degree() const { return _deg; }

private:
    size_t _deg, _cap;
    std::unique_ptr<T[]> _ptr;

    size_t _d_deg, _d_cap;
    std::unique_ptr<size_t[]> _d_ptr;

    // resize only affects the capacity, not the degree
    bool resize(const size_t size) {
        // resize should not delete existing info
        if (size <= _cap) return false;

        // reallocation most thread safe
        auto temp = std::make_unique<T[]>(size);

        if constexpr (std::is_nothrow_move_assignable_v<
                          std::unique_ptr<T[]>>) {
            std::move(_ptr.get(), _ptr.get() + _cap, temp.get());
        } else {
            std::copy(_ptr.get(), _ptr.get() + _cap, temp.get());
        }

        _ptr = std::move(temp);
        _cap = size;

        return true;
    }

    // resize only affects the capacity, not the degree
    bool resize_d(const size_t size) {
        // resize should not delete existing info
        if (size <= _d_cap) return false;

        // reallocation most thread safe
        auto temp = std::make_unique<size_t[]>(size);

        if constexpr (std::is_nothrow_move_assignable_v<
                          std::unique_ptr<size_t[]>>) {
            std::move(_d_ptr.get(), _d_ptr.get() + _d_cap, temp.get());
        } else {
            std::copy(_d_ptr.get(), _d_ptr.get() + _d_cap, temp.get());
        }

        _d_ptr = std::move(temp);
        _d_cap = size;

        return true;
    }
};
