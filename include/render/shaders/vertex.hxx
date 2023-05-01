#pragma once

#include <array>
#include <cassert>
#include <concepts>

namespace GFX
{
template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <Numeric T> struct Vertex
{
    std::array<T, 2> position;
    std::array<T, 3> color;
    std::array<T, 2> textureCoordinates;

    constexpr Vertex() = default;

    constexpr Vertex(const std::array<T, 2> &position, const std::array<T, 3> &color,
                     const std::array<T, 2> &textureCoordinates)
        : position(position), color(color), textureCoordinates(textureCoordinates)
    {
    }

    constexpr bool operator==(const Vertex &other) const
    {
        return position == other.position && color == other.color && textureCoordinates == other.textureCoordinates;
    }

    constexpr bool operator!=(const Vertex &other) const
    {
        return *this != other;
    }
};

template <Numeric T> T interpolate(const T &f0, const T &f1, const double t)
{
    assert(t >= 0.0);
    assert(t <= 1.0);
    return f0 + (f1 - f0) * t;
}

template <Numeric T> Vertex<T> interpolate(const Vertex<T> &v0, const Vertex<T> &v1, const double t)
{
    return {{interpolate(v0.position[0], v1.position[0], t), interpolate(v0.position[1], v1.position[1], t)},
            {interpolate(v0.color[0], v1.color[0], t), interpolate(v0.color[1], v1.color[1], t),
             interpolate(v0.color[2], v1.color[2], t)},
            {interpolate(v0.textureCoordinates[0], v1.textureCoordinates[0], t),
             interpolate(v0.textureCoordinates[1], v1.textureCoordinates[1], t)}};
}
} // namespace GFX
