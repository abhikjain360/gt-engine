#pragma once

#include <cstddef>
#include <memory>

template <typename T>
class resizeable_array {
public:
    constexpr resizeable_array() noexcept
        : _deg(0), _cap(2), _ptr(std::make_unique<T[]>(2)) {}

    constexpr resizeable_array(const size_t capacity) noexcept
        : _deg(0), _cap(capacity), _ptr(std::make_unique<T[]>(capacity)) {}

    resizeable_array(std::unique_ptr<T> p, const size_t size)
        : _deg(size), _cap(next2(size)), _ptr(std::move(p)) {}

    constexpr size_t degree() const noexcept { return _deg; }
    constexpr size_t capacity() const noexcept { return _cap; }

    void set_degree(const size_t degree) noexcept { _deg = degree; }

    void put(const size_t index, const T& t) {
        if (index >= _cap) this->resize(index + 1);
        _ptr[index] = t;
        ++_deg;
    }

    void remove(const size_t index, const T& t = 0) {
        if (index >= _cap) return;
        _ptr[index] = t;
        --_deg;
    }

    T operator[](const size_t index) { return _ptr[index]; }

private:
    size_t _deg, _cap;
    std::unique_ptr<T[]> _ptr;

    // utility functon
    constexpr size_t next2(size_t size) const noexcept {
        size_t lt = 2;
        while (lt <= size) lt <<= 1;
        return lt;
    }

    constexpr size_t prev2(size_t size) const noexcept {
        size_t lt = 2;
        while (lt <= size) lt <<= 1;
        lt >>= 1;
        return lt;
    }

    // Resize only affects the capacity, not the degree. Increases capacity to
    // next power of 2
    bool resize(const size_t size) {
        // resize should not delete existing info
        if (size <= _cap) return false;

        size_t lt = next2(size);

        this->trim(lt);
    }

    void trim(const size_t size) {
        // reallocation most thread safe
        auto temp = std::make_unique<T[]>(size);

        if constexpr (std::is_nothrow_move_assignable_v<std::unique_ptr<T[]>>) {
            std::move(_ptr.get(), _ptr.get() + _cap, temp.get());
        } else {
            std::copy(_ptr.get(), _ptr.get() + _cap, temp.get());
        }

        _ptr = std::move(temp);
        _cap = size;

        return;
    }
};
