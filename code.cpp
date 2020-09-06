#include <iostream>
#include <utility>

#include "graph.hpp"

int main() {
    auto V = std::make_unique<vertex[]>(10);
    for (int i = 0; i < 9; ++i) {
        V[i].set_id(i + 1);
        V[i].join(i + 2);
    }
    V[9].set_id(10);
    V[9].join(1);
    graph G(std::move(V), 10);
    auto E = std::make_unique<edge[]>(10);
    for(size_t i=0;i<7;i++)
    {
        E[i] = {i,2,1} ;
    }

    return 0;
}
