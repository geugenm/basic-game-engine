#pragma once

#include "render/shapes/position_2d.hxx"

#include <cmath>
#include <cstddef>
#include <random>
#include <stdexcept>

struct Box2D {
    std::size_t width  = 0;
    std::size_t height = 0;

    constexpr Box2D() = default;

    Box2D(const std::size_t& new_width, const std::size_t& new_height) {
        set_dimensions(new_width, new_height);
    }

    Box2D(const std::int32_t& new_width, const std::int32_t& new_height) {
        set_dimensions(new_width, new_height);
    }

    Box2D(const Box2D& other) {
        width  = other.width;
        height = other.height;
    }

    Box2D& operator=(const Box2D& other) {
        if (this == &other) {
            return *this;
        }
        width  = other.width;
        height = other.height;
        return *this;
    }

    Box2D(Box2D&& other) noexcept {
        width        = other.width;
        height       = other.height;
        other.width  = 0;
        other.height = 0;
    }

    Box2D& operator=(Box2D&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        width        = other.width;
        height       = other.height;
        other.width  = 0;
        other.height = 0;
        return *this;
    }

    bool operator!=(const Box2D& other) const {
        return (width != other.width) || (height != other.height);
    }

    bool operator<(const Box2D& other) const {
        return area() < other.area();
    }

    bool operator>(const Box2D& other) const {
        return area() > other.area();
    }

    std::ostream& operator<<(std::ostream& os) const {
        os << "Rectangle: " << width << " x " << height;
        return os;
    }

    ~Box2D() = default;

    [[nodiscard]] std::size_t area() const {
        return width * height;
    }

    [[nodiscard]] std::size_t perimeter() const {
        return 2 * (width + height);
    }

    void set_dimensions(const std::size_t& new_width, const std::size_t& new_height) {
        if (new_width == 0 || new_height == 0) {
            throw std::invalid_argument("Width and height can't be 0");
        }
        width  = new_width;
        height = new_height;
    }

    void set_dimensions(const std::int32_t& new_width, const std::int32_t& new_height) {
        if (new_width <= 0 || new_height <= 0) {
            throw std::invalid_argument("Width and height must be positive integers.");
        }
        width  = static_cast<std::size_t>(new_width);
        height = static_cast<std::size_t>(new_height);
    }

    [[nodiscard]] bool contains(const Position2D& point) const {
        return point.x >= 0 && point.x < static_cast<int32_t>(width) && point.y >= 0 &&
               point.y < static_cast<int32_t>(height);
    }

    [[nodiscard]] Position2D random_point() const {
        static std::random_device              rd;
        static std::mt19937                    gen(rd());
        std::uniform_int_distribution<int32_t> dist_x(0, static_cast<int32_t>(width) - 1);
        std::uniform_int_distribution<int32_t> dist_y(0, static_cast<int32_t>(height) - 1);
        return { dist_x(gen), dist_y(gen) };
    }
};
