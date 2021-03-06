#pragma once

#include <cstddef>
#include <iostream>

struct edge
{
    float weight;
    size_t src, dest;

    constexpr edge() noexcept
      : weight(0)
      , src(0)
      , dest(0)
    {}
    constexpr edge(const size_t s, const size_t d, const float w = 1) noexcept
      : weight(w)
      , src(s)
      , dest(d)
    {}
    constexpr edge(const float t) noexcept
      : weight(t)
      , src(t)
      , dest(t)
    {}

    bool operator<(const edge& e) const { return dest < e.dest; }
    bool operator<=(const edge& e) const { return dest <= e.dest; }
    bool operator>=(const edge& e) const { return dest >= e.dest; }
    bool operator>(const edge& e) const { return dest > e.dest; }
    bool operator==(const edge& e) const
    {
        return weight == e.weight && src == e.src && dest == e.dest;
    }
    bool operator!=(const edge& e) const
    {
        return weight != e.weight || src != e.src || dest != e.dest;
    }
};

constexpr
std::ostream&
operator<<(std::ostream& out, const edge& e)
{
    return out;
}
