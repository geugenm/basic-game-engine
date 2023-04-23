#pragma once

#include <cstdint>

#include <algorithm>
#include <cmath>
#include <cstdint>

#pragma pack(push, 1)

struct Color {
    std::uint8_t red;
    std::uint8_t green;
    std::uint8_t blue;

    Color() = default;

    Color(std::uint8_t r, std::uint8_t g, std::uint8_t b)
        : red(r), green(g), blue(b)
    {}

    Color(const Color&) = default;

    Color(Color&&) = default;

    Color& operator=(const Color&) = default;

    Color& operator=(Color&&) = default;

    bool operator==(const Color& other) const {
        return red == other.red && green == other.green && blue == other.blue;
    }

    bool operator!=(const Color& other) const {
        return !(*this == other);
    }

    Color operator+(const Color& other) const {
        return {
            static_cast<std::uint8_t>(std::min(
                static_cast<int>(red) + static_cast<int>(other.red), 255)),
            static_cast<std::uint8_t>(std::min(
                static_cast<int>(green) + static_cast<int>(other.green), 255)),
            static_cast<std::uint8_t>(std::min(
                static_cast<int>(blue) + static_cast<int>(other.blue), 255))
        };
    }

    Color operator-(const Color& other) const {
        return {
            static_cast<std::uint8_t>(std::max(
                static_cast<int>(red) - static_cast<int>(other.red), 0)),
            static_cast<std::uint8_t>(std::max(
                static_cast<int>(green) - static_cast<int>(other.green), 0)),
            static_cast<std::uint8_t>(std::max(
                static_cast<int>(blue) - static_cast<int>(other.blue), 0))
        };
    }

    Color operator*(float scalar) const {
        return {
            static_cast<std::uint8_t>(std::max(
                std::min(static_cast<int>(std::round(red * scalar)), 255), 0)),
            static_cast<std::uint8_t>(std::max(
                std::min(static_cast<int>(std::round(green * scalar)), 255),
                0)),
            static_cast<std::uint8_t>(std::max(
                std::min(static_cast<int>(std::round(blue * scalar)), 255), 0))
        };
    }
};

#pragma pack(pop)
