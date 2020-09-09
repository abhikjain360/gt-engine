#pragma once

#include "resizeable_array.hpp"

class index_keeper {
public:
    /* Constructors */
    index_keeper(const size_t capacity) : arr(capacity) {}

    index_keeper() { index_keeper(2); }

    /* Copy Constructors */
    index_keeper(index_keeper& idk) : arr(std::move(idk.arr)) {}
    index_keeper(index_keeper&& idk) : arr(std::move(idk.arr)) {}

    constexpr size_t capacity() const noexcept { return arr.capacity(); }
    constexpr size_t degree() const noexcept { return arr.degree(); }

    constexpr size_t operator[](const size_t index) const {
        assert(index < arr.degree());
        return arr[index];
    }

    constexpr void add(const size_t index, const size_t val) { arr.put(index, val); }

    constexpr void remove(const size_t index) {
        assert(index < arr.degree());
        arr.remove(index);
    }

private:
    resizeable_array<size_t> arr;
};
