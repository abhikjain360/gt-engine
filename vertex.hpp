#pragma once

#include <algorithm>
#include <cassert>
#include <utility>

#include "cached_array.hpp"
#include "edge.hpp"

template <typename T>
using edge_list = cached_array<T>;

class graph;

class vertex {
public:
    /* Constructors */
    constexpr vertex()
      : m_id(0)
      , wgt(1)
      , in_edges()
      , out_edges()
      , edges_sorted(false)
    {}

    constexpr vertex(const size_t id, const float weight = 1) noexcept
      : m_id(id)
      , wgt(weight)
      , in_edges()
      , out_edges()
      , edges_sorted(false)
    {}

    vertex(const size_t id,
           std::unique_ptr<edge[]> ptr,
           const size_t size,
           const float weight = 1)
      : m_id(id)
      , wgt(weight)
      , edges_sorted(false)
    {
        for (int i = 0; i < size; ++i)
        {
            if (ptr[i].src != m_id)
                in_edges.add(ptr[i]);
            else
            {
                // failing this means that neither src nor dest is current
                // vertex thus this edge can not be added to this vertex
                assert(ptr[i].src == m_id);
                out_edges.add(ptr[i]);
            }
        }
    }

    /* Copy Contuctor */
    vertex(vertex& v)
      : m_id(v.m_id)
      , wgt(v.wgt)
      , in_edges(v.in_edges)
      , out_edges(v.out_edges)
      , edges_sorted(v.edges_sorted)
    {}
    vertex(vertex&& v)
      : m_id(v.m_id)
      , wgt(v.wgt)
      , in_edges(std::move(v.in_edges))
      , out_edges(std::move(v.out_edges))
      , edges_sorted(v.edges_sorted)
    {}

    /* Getters */
    constexpr size_t id() const noexcept { return m_id; }
    constexpr float weight() const noexcept { return wgt; }
    constexpr size_t capacity() const noexcept
    {
        return in_edges.capacity() + out_edges.capacity();
    }
    constexpr size_t in_degree() const noexcept { return in_edges.degree(); }
    constexpr size_t out_degree() const noexcept { return out_edges.degree(); }
    constexpr size_t degree() const noexcept { return in_degree() + out_degree(); }

    /* Setters */
    constexpr void set_weight(const float weight) { wgt = weight; }
    constexpr void set_id(const size_t id) { m_id = id; }

    constexpr void join(const edge& e)
    {
        if (e.src != m_id)
            in_edges.add(e);
        else
        {
            assert(e.src == m_id);
            out_edges.add(e);
        }

        edges_sorted = false;
    }

    constexpr void unjoin(const edge& e)
    {
        if (e.src != m_id)
            in_edges.remove(e, [](const edge& a, const edge& b) -> bool {
                return a.src == b.src;
            });
        else
        {
            assert(e.src == m_id);
            out_edges.remove(e, [](const edge& a, const edge& b) -> bool {
                return a.dest == b.dest;
            });
        }
    }

    /* Assignment operator */
    constexpr vertex& operator=(vertex&& v)
    {
        m_id         = v.m_id;
        wgt          = v.wgt;
        in_edges     = std::move(v.in_edges);
        out_edges    = std::move(v.out_edges);
        edges_sorted = v.edges_sorted;

        v.m_id = v.wgt = 0;

        return *this;
    }

    constexpr vertex& operator=(const vertex& v)
    {
        m_id         = v.m_id;
        wgt          = v.wgt;
        in_edges     = v.in_edges;
        out_edges    = v.out_edges;
        edges_sorted = v.edges_sorted;

        return *this;
    }

    // UNTESTED
    constexpr void sort_edges()
    {
        in_edges.sort(
          [](const edge& a, const edge& b) -> bool { return a.src > b.src; });
        out_edges.sort(
          [](const edge& a, const edge& b) -> bool { return a.dest > b.dest; });

        edges_sorted = true;
    }

private:
    size_t m_id;
    float wgt;
    edge_list<edge> in_edges, out_edges;
    bool edges_sorted = false;

    friend class graph;
};

constexpr vertex&
operator+(vertex& v, const edge& e)
{
    v.join(e);
    return v;
}

constexpr vertex&
operator-(vertex& v, const edge& e)
{
    v.unjoin(e);
    return v;
}
