#pragma once

#include "resizeable_array.hpp"

template <typename T>
using _ptr_t = resizeable_array<T>;

// this should ensure that del-cache does not have empty indices between data

template <typename T>
class cached_array {
public:
    /* Constructors */
    // UNTESTED
    constexpr cached_array() noexcept : ptr(), d_ptr(){};
    // UNTESTED
    constexpr cached_array(std::unique_ptr<T[]> p, const size_t size) noexcept
        : ptr(p, size), d_ptr(size){};

    /* Getters */
    // UNTESTED
    constexpr size_t degree() const noexcept { return ptr.degree(); }
    // UNTESTED
    constexpr size_t capacity() const noexcept { return ptr.capacity(); }

    /* editting the array values */
    // UNTESTED
    void add(const T& t) {
        if (d_ptr.degree() > 0)
            ptr.put(d_ptr.pop(), t);
        else
            ptr.put(ptr.degree(), t);
    }

    // UNTESTED
    template <typename Compare>
    bool remove(const T& t, Compare compare) {
        for (size_t i = 0; i < ptr.capacity(); ++i) {
            if (compare(ptr[i], t)) {
                ptr.remove(i);
                d_ptr.push(i);
            }
        }
    }

    /* for assignments */
    // UNTESTED
    cached_array<T>& operator=(cached_array<T>&& arr) {
        ptr   = std::move(arr.ptr);
        d_ptr = std::move(arr.d_ptr);

        return *this;
    }

    // UNTESTED
    cached_array<T>& operator=(const cached_array<T>& arr) {
        ptr   = arr.ptr;
        d_ptr = arr.d_ptr;
    }

private:
    _ptr_t<T> ptr;
    _ptr_t<size_t> d_ptr;
};
