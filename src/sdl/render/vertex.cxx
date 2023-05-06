#include "vertex.hxx"

namespace Engine
{
constexpr Vertex::Vertex(std::ptrdiff_t x, std::ptrdiff_t y) : x_(x), y_(y) {}

constexpr Vertex::Vertex(const std::array<std::ptrdiff_t, 2>& coordinates)
    : x_(coordinates[0]), y_(coordinates[1])
{
}

constexpr void Vertex::set(std::ptrdiff_t x, std::ptrdiff_t y)
{
    x_ = x;
    y_ = y;
}

constexpr std::ptrdiff_t Vertex::get_x() const
{
    return x_;
}

constexpr std::ptrdiff_t Vertex::get_y() const
{
    return y_;
}

template <std::size_t I> constexpr std::ptrdiff_t Vertex::get() const
{
    static_assert(I < 2, "Index out of bounds");
    if constexpr (I == 0)
    {
        return x_;
    }
    else
    {
        return y_;
    }
}

template std::ptrdiff_t Vertex::get<0>() const;
template std::ptrdiff_t Vertex::get<1>() const;
} // namespace Engine
