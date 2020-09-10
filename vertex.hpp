#pragma once

#include <algorithm>
#include <cassert>
#include <type_traits>
#include <utility>

#include "cached_array.hpp"
#include "edge.hpp"

template <typename T>
using edge_list = cached_array<T>;

class vertex {
public:
    /* Constructors */
    constexpr vertex() : m_id(0), wgt(1), edges() {}

    constexpr vertex(const size_t id, const float weight = 1) noexcept
        : m_id(id), wgt(weight), edges() {}

    vertex(const size_t id, std::unique_ptr<edge[]> ptr, const size_t size,
           const float weight = 1)
        : m_id(id), wgt(weight), edges(std::move(ptr), size) {}

    /* Copy Contuctor */
    vertex(vertex& v) : m_id(v.m_id), wgt(v.wgt), edges(v.edges) {}
    vertex(vertex&& v) : m_id(v.m_id), wgt(v.wgt), edges(std::move(v.edges)) {}

    /* Getters */
    constexpr size_t id() const noexcept { return m_id; }
    constexpr float weight() const noexcept { return wgt; }
    constexpr size_t capacity() const noexcept { return edges.capacity(); }
    constexpr size_t degree() const noexcept { return edges.degree(); }

    /* Setters */
    constexpr void set_weight(const float weight) { wgt = weight; }
    constexpr void set_id(const size_t id) { m_id = id; }

    void join(const edge& e) { edges.add(e); }
    void unjoin(const edge& e) { edges.remove(e); }

    const edge& operator[](const size_t index) {
        assert(index < edges.capacity());
        return edges[index];
    }

    vertex& operator=(vertex&& v) {
        m_id  = v.m_id;
        wgt   = v.wgt;
        edges = std::move(v.edges);

        return *this;
    }

    vertex& operator=(const vertex& v) {
        m_id  = v.m_id;
        wgt   = v.wgt;
        edges = v.edges;

        return *this;
    }

    const edge next(size_t& i) const {
        while (edges[i].src == 0 && i < edges.capacity()) ++i;
        if (i == edges.capacity())
            return -1;
        else
            return edges[i];
    }

private:
    size_t m_id;
    float wgt;
    edge_list<edge> edges;
};
