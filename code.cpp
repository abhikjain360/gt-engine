#include <iostream>
#include <utility>

#include "vertex.hpp"

template <typename T>
using vec = cached_array<T>;

int main() {
    std::unique_ptr<int[]> arr_old = std::make_unique<int[]>(9);
    for (size_t i = 1; i < 10; ++i) arr_old[i] = i + 1;

    vec<int> v(std::move(arr_old), 10);

    for (size_t i = 1; i < 8; ++i) v.add(i);
    for (int i = 1; i < 9; ++i) v.remove(i);

    std::unique_ptr<joiner[]> arr = std::make_unique<joiner[]>(10);
    for (size_t i = 1; i < 10; ++i) arr[i] = {i, static_cast<float>(i + 1)};

    vertex v1(1, std::move(arr), 10);

    for (size_t i = 12; i < 18; ++i) v1.join(i);
    for (size_t i = 1; i < 9; ++i) v1.unjoin(i);

    v1.join(10, 14);

    return 0;
}
