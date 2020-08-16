#pragma once

#include <cstddef>
#include <memory>

template <typename T>
class resizeable_array {
public:
    constexpr resizeable_array() noexcept
        : deg(0), cap(2), ptr(std::make_unique<T[]>(2)) {}

    constexpr resizeable_array(const size_t capacity) noexcept
        : deg(0), cap(capacity), ptr(std::make_unique<T[]>(capacity)) {}

    resizeable_array(std::unique_ptr<T> p, const size_t size)
        : deg(size), cap(next2(size)), ptr(std::move(p)) {}

    constexpr size_t degree() const noexcept { return deg; }
    constexpr size_t capacity() const noexcept { return cap; }

    void set_degree(const size_t degree) noexcept { deg = degree; }

    void put(const size_t index, const T& t) {
        if (index >= cap) this->resize(index + 1);
        ptr[index] = t;
        ++deg;
    }

    void remove(const size_t index, const T& t = 0) {
        if (index >= cap) return;
        ptr[index] = t;
        --deg;
    }

    T& pop() { return ptr[deg--]; }
    void push(const T& t) { ptr[deg++] = t; }

    const T& operator[](const size_t index) const { return ptr[index]; }

    resizeable_array<T>& operator=(resizeable_array<T>&& arr) {
        // setting values of the LHS
        deg = arr.deg;
        cap = arr.cap;
        ptr = std::move(arr._ptr);
    }

    resizeable_array<T>& operator=(const resizeable_array<T>& arr) {
        deg = arr.deg;
        cap = arr.cap;
        for (size_t i = 0; i < cap; ++i) ptr[i] = arr.ptr[i];
    }

private:
    size_t deg, cap;
    std::unique_ptr<T[]> ptr;

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
        if (size <= cap) return false;

        size_t lt = next2(size);

        this->trim(lt);
    }

    void trim(const size_t size) {
        // reallocation most thread safe
        auto temp = std::make_unique<T[]>(size);

        if constexpr (std::is_nothrow_move_assignable_v<std::unique_ptr<T[]>>) {
            std::move(ptr.get(), ptr.get() + cap, temp.get());
        } else {
            std::copy(ptr.get(), ptr.get() + cap, temp.get());
        }

        ptr = std::move(temp);
        cap = size;

        return;
    }
};
