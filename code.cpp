#include <cstdlib>
#include <iostream>

#include "graph.hpp"

int
main()
{
    // graph constructor 1
    graph g1(10);

    // graph constructor 2
    std::unique_ptr<vertex[]> p = std::make_unique<vertex[]>(10);
    for (int i = 0; i < 10; ++i)
        p[i].set_id(i + 1);
    graph g2(std::move(p), 10);

    // graph constructor 3
    std::unique_ptr<edge[]> e = std::make_unique<edge[]>(20);
    for (size_t i = 0; i < 20; ++i)
        e[i] = { static_cast<size_t>(rand() % 10 + 1),
                 static_cast<size_t>(rand() % 10 + 1),
                 static_cast<float>(rand() % 100 + 1) };
    graph g3(std::move(e), 20);
    g3.sort_edges();

    // joining edges
    g1.join({ 1, 2, 1 });
    g1.join({ 2, 3, 1 });
    g1.join({ 3, 4, 1 });
    g1.join({ 4, 5, 1 });

    // unjoining edges
    g1.unjoin({ 1, 2, 1 });
    g1.unjoin({ 2, 3, 1 });

    edge e2[2] = { { 6, 1, 1 }, { 6, 2, 1 } };
    std::unique_ptr<edge[]> e1(e2);
    vertex v1(6, std::move(e1), 2);
    g1.join(v1);
    g1.unjoin(v1.id());

    return 0;
}
