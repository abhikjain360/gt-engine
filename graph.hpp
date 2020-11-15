#pragma once

#include <algorithm>

#include "edge.hpp"
#include "vertex.hpp"

class graph {
public:
    /* Constructors */
    graph(const size_t deg)
      : V()
      , E()
    {
        V.reserve(deg);
        E.reserve(deg);

        for (size_t i = 1; i <= deg; ++i)
        {
            V.push_back(i);
        }
    }

    graph(std::vector<vertex>& ptr)
    {
        size_t size = ptr.size();

        E.reserve(size);

        for (size_t i = 0; i < size; ++i)
        {
            V[ptr[i].m_id].in_edges  = ptr[i].in_edges;
            V[ptr[i].m_id].out_edges = ptr[i].out_edges;

            for (size_t j = 0; j < ptr[i].in_degree(); ++j)
            {
                E.push_back({ ptr[i].in_edges[j].dest,
                              ptr[i].id(),
                              ptr[i].in_edges[j].weight });
            }

            for (size_t j = 0; j < ptr[i].out_degree(); ++j)
            {
                E.push_back({ ptr[i].out_edges[j].src,
                              ptr[i].id(),
                              ptr[i].out_edges[j].weight });
            }
        }
    }

    graph(std::vector<edge>&& p)
    {
        size_t size = p.size();

        E = std::move(p);

        size_t max_id = 0;
        for (size_t i = 0; i < size; i++)
        {
            if (E[i].src > max_id)
            {
                max_id = E[i].src;
                V.push_back(vertex(E[i].src));
            }
            if (E[i].dest > max_id)
            {
                max_id = E[i].dest;
                V.push_back(vertex(E[i].dest));
            }
            V[E[i].src].join(E[i]);
            V[E[i].dest].join(E[i]);
        }
    }

    /* Copy Constructor */
    graph(const graph& G)
      : V(G.V)
      , E(G.E)
    {}

    /* Move Constructor */
    graph(graph&& G)
      : V(std::move(G.V))
      , E(std::move(G.E))
    {}

    /* Assignment operators */
    constexpr graph& operator=(const graph& G)
    {
        V = G.V;
        E = G.E;

        return *this;
    }

    constexpr graph& operator=(graph&& G)
    {
        V = std::move(G.V);
        E = std::move(G.E);

        return *this;
    }

    /* Getters */
    constexpr size_t edge_degree() const noexcept { return E.size(); }
    constexpr size_t degree() const noexcept { return V.size(); }

    constexpr size_t degree(const size_t vertex_id) const noexcept
    {
        assert(vertex_id < V.size());
        return V[vertex_id].degree();
    }

    constexpr const vertex& operator[](const size_t vertex_id) const
    {
        return V[vertex_id];
    }

    constexpr const edge& get_edge(const size_t index) const
    {
        return E[index];
    }

    template <typename Compare = bool(const edge&, const edge&)>
    constexpr void sort_edges(Compare compare = [](const edge& e1,
                                                   const edge& e2) -> bool {
        return e1.src > e2.src;
    })
    {
        std::sort(E.begin(), E.end(), compare);
    }

    // untested
    constexpr float** adjacency_matrix() const noexcept
    {
        float* temp = new float[V.size() * V.size()];
        float** arr = new float*[V.size()];

        for (size_t i = 0; i < V.size(); ++i)
            for (size_t j = 0; j < V.size(); ++j)
                arr[i] = &temp[i * V.size() + j];

        for (size_t i = 0; i < E.size(); ++i)
            arr[E[i].src][E[i].dest] += E[i].weight;

        return arr;
    }

    /* adding new elements */
    constexpr void join(const edge& e)
    {
        E.push_back(e);
        V[e.src].join(e);
        V[e.dest].join(e);
    }

    constexpr void unjoin(const edge& e)
    {
        assert(e.src < V.size() && e.dest < V.size());

        size_t size = E.size();

        for (int i = 0; i < size; ++i)
        {
            if (E[i].src == e.src && E[i].dest == e.dest)
            {
                E.erase(E.begin() + i);

                V[e.src].unjoin(e);
                V[e.dest].unjoin(e);

                break;
            }
        }
    }

    constexpr void operator+=(const edge& e) { this->join(e); }
    constexpr void operator-=(const edge& e) { this->unjoin(e); }

    std::vector<vertex> V;
    std::vector<edge> E;
};

inline graph
operator+(graph G, const edge& e)
{
    G.join(e);
    return G;
}

inline graph
operator-(graph G, const edge& e)
{
    G.unjoin(e);
    return G;
}

constexpr std::ostream&
operator<<(std::ostream& out, const graph& G) noexcept
{
    auto arr = G.adjacency_matrix();

    for (size_t i = 0; i < G.degree(); ++i)
    {
        for (size_t j = 0; j < G.degree(); ++j)
            out << arr[i][j] << ' ';
        out << '\n';
    }

    return out;
}
