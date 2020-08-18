#pragma once

#include <utility>

#include "resizeable_array.hpp"

template <typename T>
using _ptr_t = resizeable_array<T>;

// this should ensure that del-cache does not have empty indices between data

template <typename T>
class cached_array {
public:
    /* Constructors */
    constexpr cached_array() noexcept : ptr(), d_ptr(){};
    cached_array(std::unique_ptr<T[]> p, const size_t size) noexcept
        : ptr(std::move(p), size), d_ptr(size){};

    /* Getters */
    constexpr size_t degree() const noexcept { return ptr.degree(); }
    constexpr size_t capacity() const noexcept { return ptr.capacity(); }

    /* editting the array values */
    void add(const T& t) {
        if (d_ptr.degree() > 0)
            ptr.put(d_ptr.pop().value(), t);
        else
            ptr.put(ptr.degree(), t);
    }

    template <typename Compare = bool(const T&, const T&)>
    void remove(
        const T& t,
        Compare compare = [](const T& a, const T& b) -> bool { return a == b; }) {
        for (size_t i = 0; i < ptr.capacity(); ++i) {
            if (compare(ptr[i], t)) {
                ptr.remove(i);
                d_ptr.push(i);
            }
        }
    }

    template <typename S = T, typename Compare = bool(const T&, const S&)>
    void remove(
        const S& s,
        Compare compare = [](const T& a, const T& b) -> bool { return a == b; }) {
        for (size_t i = 0; i < ptr.capacity(); ++i) {
            if (compare(ptr[i], s)) {
                ptr.remove(i);
                d_ptr.push(i);
            }
        }
    }

    /* for assignments */
    cached_array<T>& operator=(cached_array<T>&& arr) {
        ptr   = std::move(arr.ptr);
        d_ptr = std::move(arr.d_ptr);

        return *this;
    }

    cached_array<T>& operator=(const cached_array<T>& arr) {
        ptr   = arr.ptr;
        d_ptr = arr.d_ptr;

        return *this;
    }

private:
    _ptr_t<T> ptr;
    _ptr_t<size_t> d_ptr;
};
