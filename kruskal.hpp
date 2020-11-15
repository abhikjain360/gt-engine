#pragma once

#include "graph.hpp"
#include <cstdlib>
#include <vector>

bool
compare(const edge& e1, const edge& e2)
{
    return e1.weight < e2.weight;
}

void
makeSet(int v, std::vector<int>& parent)
{
    parent[v] = v;
}

int
findSet(int v, std::vector<int>& parent)
{
    if (v == parent[v])
    {
        return v;
    }
    return parent[v] = findSet(parent[v], parent);
}

bool
unionSet(int a, int b, std::vector<int>& parent, std::vector<int>& depth)
{
    a = findSet(a, parent);
    b = findSet(b, parent);
    if (a != b)
    {
        if (depth[a] < depth[b])
        {
            std::swap(a, b);
        }
        parent[b] = a;
        depth[a] += depth[b];
        return true;
    }
    return false;
}

graph
KruskalsMinimumSpaningTree(graph& g)
{
    int degree = g.degree();

    graph mst(degree);

    std::vector<int> parent;
    parent.resize(degree, -1);

    std::vector<int> depth;
    depth.resize(degree, 1);

    for (int i = 0; i < degree; i++)
    {
        makeSet(i, parent);
    }
    for (int i = 0; i < g.edge_degree(); i++)
    {
        if (unionSet(g.E[i].src, g.E[i].dest, parent, depth))
        {
            mst.join(g.E[i]);
        }
    }

    return mst;
}
