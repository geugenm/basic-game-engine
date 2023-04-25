#pragma once

#include "render/shapes/position_2d.hxx"

#include <cmath>
#include <cstddef>
#include <random>
#include <stdexcept>

struct BoundingBox {
    std::size_t width  = 0;
    std::size_t height = 0;

    constexpr BoundingBox() = default;

    BoundingBox(const std::size_t& new_width, const std::size_t& new_height) {
        set_dimensions(new_width, new_height);
    }

    BoundingBox(const std::int32_t& new_width, const std::int32_t& new_height) {
        set_dimensions(new_width, new_height);
    }

    BoundingBox(const Position2D& start, const Position2D& end) {
        width  = static_cast<size_t>(std::abs(end.x - start.x + 1));
        height = static_cast<size_t>(std::abs(end.y - start.y + 1));
    }

    BoundingBox(const BoundingBox& other) {
        width  = other.width;
        height = other.height;
    }

    BoundingBox& operator=(const BoundingBox& other) {
        if (this == &other) {
            return *this;
        }
        width  = other.width;
        height = other.height;
        return *this;
    }

    BoundingBox(BoundingBox&& other) noexcept {
        width        = other.width;
        height       = other.height;
        other.width  = 0;
        other.height = 0;
    }

    BoundingBox& operator=(BoundingBox&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        width        = other.width;
        height       = other.height;
        other.width  = 0;
        other.height = 0;
        return *this;
    }

    bool operator!=(const BoundingBox& other) const {
        return (width != other.width) || (height != other.height);
    }

    bool operator<(const BoundingBox& other) const {
        return area() < other.area();
    }

    bool operator>(const BoundingBox& other) const {
        return area() > other.area();
    }

    ~BoundingBox() = default;

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

    [[nodiscard]] std::string string() const {
        return std::string("[") + std::to_string(width) + "x" + std::to_string(height) + "]";
    }
};
