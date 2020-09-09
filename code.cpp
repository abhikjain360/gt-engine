#include <iostream>
#include <utility>

#include "graph.hpp"

int main() {
    // testing resizeable_array
    resizeable_array<int> x;
    resizeable_array<int> y(10);
    std::unique_ptr<int[]> z = std::make_unique<int[]>(10);

    for (int i = 20; i < 30; ++i) {
        z[i - 20] = i;
        y.push(i - 19);
        y.push(i + 10);
    }

    resizeable_array t(std::move(z), 10);
    t.push(13);
    t.pop();

    resizeable_array a(t);
    resizeable_array b(std::move(t));

	// testing cached_array

    return 0;
}
