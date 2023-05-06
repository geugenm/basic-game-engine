#include <array>

namespace Engine
{
class Vertex
{
public:
    constexpr explicit Vertex(std::ptrdiff_t x = 0, std::ptrdiff_t y = 0);;
    constexpr explicit Vertex(const std::array<std::ptrdiff_t, 2>& coordinates);;

    constexpr void set(std::ptrdiff_t x, std::ptrdiff_t y);

    [[nodiscard]] constexpr std::ptrdiff_t get_x() const;
    [[nodiscard]] constexpr std::ptrdiff_t get_y() const;

    // Tuple-like interface
    template <std::size_t I> [[nodiscard]] constexpr std::ptrdiff_t get() const;

private:
    std::ptrdiff_t x_;
    std::ptrdiff_t y_;
};
} // namespace Engine
