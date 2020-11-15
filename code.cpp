#include "graph.hpp"
#include "kruskal.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

int
main()
{
    graph g1(5);
    g1.join({ 0, 1, 1 });
    g1.join({ 1, 2, 7 });
    g1.join({ 2, 3, 4 });
    g1.join({ 3, 0, 9 });
    g1.join({ 0, 4, 5 });
    g1.join({ 1, 4, 2 });
    g1.join({ 2, 4, 7 });
    g1.join({ 3, 4, 1 });
    g1.sort_edges(compare);
    graph g2 = KruskalsMinimumSpaningTree(g1);
    for (int i = 0; i < g2.edge_degree(); i++)
    {
        cout << g2.E[i].src << " " << g2.E[i].dest << " " << g2.E[i].weight
             << endl;
    }
}
