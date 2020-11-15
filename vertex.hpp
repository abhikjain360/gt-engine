#pragma once

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

#include "edge.hpp"

class graph;

class vertex {
public:
    /* Constructors */
    vertex()
      : m_id(0)
      , wgt(1)
      , in_edges()
      , out_edges()
    {}

    vertex(const size_t id, const float weight = 1) noexcept
      : m_id(id)
      , wgt(weight)
      , in_edges()
      , out_edges()
    {}

    vertex(const size_t id, std::vector<edge> ptr, const float weight = 1)
      : m_id(id)
      , wgt(weight)
    {
        auto size = ptr.size();
        in_edges.reserve(size);
        out_edges.reserve(size);
        for (int i = 0; i < size; ++i)
        {
            if (ptr[i].src != m_id)
                in_edges.push_back(ptr[i]);
            else
            {
                // failing this means that neither src nor dest is current
                // vertex thus this edge can not be added to this vertex
                assert(ptr[i].src == m_id);
                out_edges.push_back(ptr[i]);
            }
        }
    }

    /* Copy Contuctor */
    vertex(vertex& v)
      : m_id(v.m_id)
      , wgt(v.wgt)
      , in_edges(v.in_edges)
      , out_edges(v.out_edges)
    {}
    vertex(vertex&& v)
      : m_id(v.m_id)
      , wgt(v.wgt)
      , in_edges(std::move(v.in_edges))
      , out_edges(std::move(v.out_edges))
    {}

    /* Getters */
    constexpr size_t id() const noexcept { return m_id; }
    constexpr float weight() const noexcept { return wgt; }
    constexpr size_t capacity() const noexcept
    {
        return in_edges.capacity() + out_edges.capacity();
    }
    constexpr size_t in_degree() const noexcept { return in_edges.size(); }
    constexpr size_t out_degree() const noexcept { return out_edges.size(); }
    constexpr size_t degree() const noexcept
    {
        return in_degree() + out_degree();
    }

    /* Setters */
    constexpr void set_weight(const float weight) { wgt = weight; }
    constexpr void set_id(const size_t id) { m_id = id; }

    constexpr void join(const edge& e)
    {
        if (e.src != m_id)
            in_edges.push_back(e);
        else
        {
            assert(e.src == m_id);
            out_edges.push_back(e);
        }
    }

    constexpr void unjoin(const edge& e)
    {
        if (e.src == m_id)
        {
            for (int i = 0; i < out_edges.size(); ++i)
            {
                if (out_edges[i].dest == e.dest)
                {
                    out_edges.erase(out_edges.begin() + i);
                    break;
                }
            }
        }
        else if (e.dest == m_id)
        {
            for (int i = 0; i < in_edges.size(); ++i)
            {
                if (in_edges[i].src == e.src)
                {
                    in_edges.erase(in_edges.begin() + i);
                    break;
                }
            }
        }
    }

    /* Assignment operator */
    constexpr vertex& operator=(vertex&& v)
    {
        m_id         = v.m_id;
        wgt          = v.wgt;
        in_edges     = std::move(v.in_edges);
        out_edges    = std::move(v.out_edges);

        v.m_id = v.wgt = 0;

        return *this;
    }

    constexpr vertex& operator=(const vertex& v)
    {
        m_id         = v.m_id;
        wgt          = v.wgt;
        in_edges     = v.in_edges;
        out_edges    = v.out_edges;

        return *this;
    }

    // UNTESTED
    void sort_in_edges()
    {
        std::sort(
          in_edges.begin(),
          in_edges.end(),
          [](const edge& a, const edge& b) -> bool { return a.src > b.src; });
    }
    void sort_out_edges()
    {
        std::sort(
          out_edges.begin(),
          out_edges.end(),
          [](const edge& a, const edge& b) -> bool { return a.dest > b.dest; });
    }

private:
    size_t m_id;
    float wgt;
    std::vector<edge> in_edges, out_edges;

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
