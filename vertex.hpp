#pragma once

#include <algorithm>
#include <cassert>
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

    constexpr void join(const edge& e) { edges.add(e); }
    constexpr void unjoin(const edge& e) { edges.remove(e); }

    constexpr const edge& operator[](const size_t index) const noexcept {
        assert(index < edges.capacity());
        return edges[index];
    }

    constexpr vertex& operator=(vertex&& v) {
        m_id  = v.m_id;
        wgt   = v.wgt;
        edges = std::move(v.edges);

        return *this;
    }

    constexpr vertex& operator=(const vertex& v) {
        m_id  = v.m_id;
        wgt   = v.wgt;
        edges = v.edges;

        return *this;
    }

    constexpr const edge next(size_t& i) const {
        while (edges[i].src == 0 && i < edges.capacity()) ++i;
        if (i == edges.capacity())
            return -1;
        else
            return edges[i++];
    }

    // UNTESTED
    void sort_edges() {
        edges.sort([](const edge& a, const edge& b) -> bool {
            if (a.src != b.src)
                return a.src > b.src;
            else
                return a.dest > b.dest;
        });

        edges_sorted = true;
    }

private:
    size_t m_id;
    float wgt;
    edge_list<edge> edges;
    bool edges_sorted = false;
};

constexpr vertex& operator+(vertex& v, const edge& e) {
    v.join(e);
    return v;
}

constexpr vertex& operator-(vertex& v, const edge& e) {
    v.unjoin(e);
    return v;
}
