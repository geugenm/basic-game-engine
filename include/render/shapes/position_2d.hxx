#pragma once

#include <cmath>
#include <random>
#include <stdexcept>
#include <utility>

struct Position2D
{
    int32_t x = 0;
    int32_t y = 0;

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

    [[nodiscard]] constexpr double length() const
    {
        return std::sqrt(x * x + y * y);
    }

    [[nodiscard]] std::string string() const
    {
        return std::string("(") + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    static Position2D generate_random(const int32_t &from, const int32_t &to)
    {
        if (from > to)
        {
            throw std::invalid_argument("Minimal generation value can't be larger than maximal.");
        }
        std::uniform_int_distribution<int32_t> distrib(from, to);
        std::random_device rd;
        std::mt19937 gen(rd());
        return {distrib(gen), distrib(gen)};
    }

    static Position2D generate_random(const Position2D &from, const Position2D &to)
    {
        Position2D x_result = generate_random(from.x, to.x);
        Position2D y_result = generate_random(from.y, to.y);
        return {x_result.x, y_result.x};
    }

    static double distance(const Position2D &p1, const Position2D &p2)
    {
        const auto dx = static_cast<double>(p2.x - p1.x);
        const auto dy = static_cast<double>(p2.y - p1.y);
        return std::sqrt(dx * dx + dy * dy);
    }

    static double dot(const Position2D &p1, const Position2D &p2)
    {
        return static_cast<double>(p1.x * p2.x + p1.y * p2.y);
    }

    static double cross(const Position2D &p1, const Position2D &p2)
    {
        return static_cast<double>(p1.x * p2.y - p1.y * p2.x);
    }

    static Position2D interpolate(const Position2D& p1, const Position2D& p2, double t)
    {
        int x = static_cast<int>(p1.x + (p2.x - p1.x) * t);
        int y = static_cast<int>(p1.y + (p2.y - p1.y) * t);
        return {x, y};
    }
};
