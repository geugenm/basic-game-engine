#include <array>

namespace Engine
{
class Vertex
{
public:
    constexpr explicit Vertex(std::ptrdiff_t x = 0, std::ptrdiff_t y = 0) : x_(x), y_(y){};
    constexpr explicit Vertex(const std::array<std::ptrdiff_t, 2>& coordinates)
        : x_(coordinates[0]), y_(coordinates[1]){};

    constexpr void set(std::ptrdiff_t x, std::ptrdiff_t y)
    {
        x_ = x;
        y_ = y;
    }

    [[nodiscard]] constexpr std::ptrdiff_t get_x() const
    {
        return x_;
    }
    [[nodiscard]] constexpr std::ptrdiff_t get_y() const
    {
        return y_;
    }

    // Tuple-like interface
    template <std::size_t I> [[nodiscard]] constexpr std::ptrdiff_t get() const
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

private:
    std::ptrdiff_t x_;
    std::ptrdiff_t y_;
};

template std::ptrdiff_t Vertex::get<0>() const;
template std::ptrdiff_t Vertex::get<1>() const;
} // namespace Engine
