#pragma once

#include <cassert>
#include <cmath>
#include <random>
#include <stdexcept>
#include <utility>

struct Position2D
{
    int32_t x = 0;
    int32_t y = 0;
    ColorRGB color;

    constexpr Position2D() = default;

    constexpr Position2D(int32_t x_, int32_t y_) : x(x_), y(y_)
    {
    }

    constexpr Position2D(const Position2D &other) = default;

    Position2D &operator=(const Position2D &other) = default;

    constexpr Position2D(Position2D &&other) noexcept : x(std::exchange(other.x, 0)), y(std::exchange(other.y, 0))
    {
    }

    Position2D &operator=(Position2D &&other) noexcept
    {
        if (this != &other)
        {
            x = std::exchange(other.x, 0);
            y = std::exchange(other.y, 0);
        }
        return *this;
    }

    friend constexpr Position2D operator-(const Position2D &left, const Position2D &right)
    {
        return {left.x - right.x, left.y - right.y};
    }

    friend constexpr bool operator==(const Position2D &left, const Position2D &right)
    {
        return left.x == right.x && left.y == right.y;
    }

    constexpr Position2D operator+(const Position2D &other) const
    {
        return {x + other.x, y + other.y};
    }

    constexpr Position2D operator*(const double scalar) const
    {
        return {static_cast<int32_t>(x * scalar), static_cast<int32_t>(y * scalar)};
    }

    constexpr Position2D &operator+=(const Position2D &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr bool operator<(const Position2D& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }

    constexpr bool operator>(const Position2D& other) const {
        return x > other.x || (x == other.x && y > other.y);
    }

    [[nodiscard]] double distance_to(const Position2D &other) const
    {
        const auto dx = static_cast<double>(x - other.x);
        const auto dy = static_cast<double>(y - other.y);
        return std::sqrt(dx * dx + dy * dy);
    }

    [[nodiscard]] std::string string() const
    {
        return std::string("(") + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};
