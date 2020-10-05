#pragma once

#include <algorithm>

#include "cached_array.hpp"
#include "edge.hpp"
#include "index_keeper.hpp"
#include "vertex.hpp"

class graph {
public:
    /* Constructors */
    graph(const size_t deg)
      : V(deg)
      , E()
      , v_loc(deg + 1)
    {
        for (size_t i = 1; i <= deg; ++i)
        {
            V.add(i);
            v_loc.add(i, i);
        }
    }

    graph(std::unique_ptr<vertex[]> p, const size_t size)
      : V(std::move(p), size)
      , E(size)
      , v_loc(size + 1)
    {
        for (size_t i = 0; i < size; ++i)
            for (size_t j = 0; j < V[i].degree(); ++j)
            {
                if (V[i][j].dest > V[i].id())
                    E.add({ V[i].id(), V[i][j].dest, V[i][j].weight });
                v_loc.add(V[i].id(), i);
            }
    }

    graph(std::unique_ptr<edge[]> p, const size_t size)
      : E(std::move(p), size)
    {
        size_t max_id = 0;
        for (size_t i = 0; i < size; i++)
        {
            if (E[i].src > max_id)
            {
                max_id = E[i].src;
                V.add(vertex(E[i].src));
                v_loc.add(E[i].src, V.degree() - 1);
            }
            if (E[i].dest > max_id)
            {
                max_id = E[i].dest;
                V.add(vertex(E[i].dest));
                v_loc.add(E[i].dest, V.degree() - 1);
            }
            V[v_loc[E[i].src]].join(E[i]);
            V[v_loc[E[i].dest]].join(E[i]);
        }
    }

    /* Copy Constructor */
    graph(const graph& G)
      : V(G.V)
      , E(G.E)
      , v_loc(G.v_loc)
    {}

    /* Move Constructor */
    graph(graph&& G)
      : V(std::move(G.V))
      , E(std::move(G.E))
      , v_loc(std::move(G.v_loc))
    {}

    /* Getters */
    constexpr size_t edge_degree() const noexcept { return E.degree(); }
    constexpr size_t degree() const noexcept { return V.degree(); }

    constexpr size_t degree(const size_t vertex_id) const noexcept
    {
        assert(vertex_id < V.degree());
        return V[vertex_id].degree();
    }

    constexpr const vertex& operator[](const size_t vertex_id) const
    {
        return V[v_loc[vertex_id]];
    }

    template <typename Compare = bool(const edge&, const edge&)>
    constexpr void sort_edges(Compare compare = [](const edge& e1,
                                                   const edge& e2) -> bool {
        return e1.src > e2.src;
    })
    {
        E.sort(compare);
    }

    constexpr float** adjacency_matrix() const noexcept
    {
        float* temp = new float[V.degree() * V.degree()];
        float** arr = new float*[V.degree()];

        for (size_t i = 0; i < V.degree(); ++i)
            for (size_t j = 0; j < V.degree(); ++j)
                arr[i] = &temp[i * V.degree() + j];

        for (size_t i = 0; i < E.degree(); ++i)
            arr[E[i].src][E[i].dest] += E[i].weight;

        return arr;
    }

    /* adding new elements */
    constexpr void join(const edge& e)
    {
        E.add(e);
        V[v_loc[e.src]].join(e);
        V[v_loc[e.dest]].join(e);
    }

    constexpr void join(const vertex& v)
    {
        // if vertex already existing in V is added assertion fails
        // for those cases add edges of the vertex using other `join` overload
        assert(v.id() > V.degree());
        V.add(v);
        size_t i = 0;
        while (i < v.capacity())
        {
            E.add(v.next(i));
        }
    }

    constexpr void unjoin(const edge& e)
    {
        E.remove(e);
        V[v_loc[e.src]].unjoin(e);
        V[v_loc[e.dest]].unjoin(e);
    }

    constexpr void unjoin(const size_t vertex_id)
    {
        V.remove(vertex_id, [](const vertex& v1, const vertex& v2) -> bool {
            return v1.id() == v2.id();
        });

        for (size_t i = 0; i < E.capacity(); ++i)
            if (E[i].src == vertex_id || E[i].dest == vertex_id)
                E.remove(i);
    }

    constexpr void unjoin(const vertex& v) { this->unjoin(v.id()); }

    constexpr void operator+=(const edge& e) { this->join(e); }
    constexpr void operator+=(const vertex& v) { this->join(v); }
    constexpr void operator-=(const edge& e) { this->unjoin(e); }
    constexpr void operator-=(const size_t vertex_id)
    {
        this->unjoin(vertex_id);
    }
    constexpr void operator-=(const vertex& v) { this->unjoin(v); }

private:
    cached_array<vertex> V;
    cached_array<edge> E;
    index_keeper v_loc;

    bool directed = true;
};

graph
operator+(graph G, const edge& e)
{
    G.join(e);
    return G;
}

graph
operator+(graph G, const vertex& v)
{
    G.join(v);
    return G;
}

graph
operator-(graph G, const edge& e)
{
    G.unjoin(e);
    return G;
}

graph
operator-(graph G, const vertex& v)
{
    G.unjoin(v);
    return G;
}

graph
operator-(graph G, const size_t vertex_id)
{
    G.unjoin(vertex_id);
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
