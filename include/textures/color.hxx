#pragma once

#include <cstdint>

#include <cstdint>
#include <algorithm>
#include <cmath>

struct Color
{
    std::uint8_t red;
    std::uint8_t green;
    std::uint8_t blue;
    std::uint8_t alpha;

    Color() = default;

    Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255)
        : red(r), green(g), blue(b), alpha(a)
    {}

    Color(const Color&) = default;

    Color(Color&&) = default;

    Color& operator=(const Color&) = default;

    Color& operator=(Color&&) = default;

    bool operator==(const Color& other) const
    {
        return red == other.red &&
               green == other.green &&
               blue == other.blue &&
               alpha == other.alpha;
    }

    bool operator!=(const Color& other) const
    {
        return !(*this == other);
    }

    Color operator+(const Color& other) const
    {
        return Color(static_cast<std::uint8_t>(std::min(static_cast<int>(red) + static_cast<int>(other.red), 255)),
                     static_cast<std::uint8_t>(std::min(static_cast<int>(green) + static_cast<int>(other.green), 255)),
                     static_cast<std::uint8_t>(std::min(static_cast<int>(blue) + static_cast<int>(other.blue), 255)),
                     static_cast<std::uint8_t>(std::min(static_cast<int>(alpha) + static_cast<int>(other.alpha), 255)));
    }

    Color operator-(const Color& other) const
    {
        return Color(static_cast<std::uint8_t>(std::max(static_cast<int>(red) - static_cast<int>(other.red), 0)),
                     static_cast<std::uint8_t>(std::max(static_cast<int>(green) - static_cast<int>(other.green), 0)),
                     static_cast<std::uint8_t>(std::max(static_cast<int>(blue) - static_cast<int>(other.blue), 0)),
                     static_cast<std::uint8_t>(std::max(static_cast<int>(alpha) - static_cast<int>(other.alpha), 0)));
    }

    Color operator*(float scalar) const
    {
        return Color(static_cast<std::uint8_t>(std::max(std::min(static_cast<int>(std::round(red * scalar)), 255), 0)),
                     static_cast<std::uint8_t>(std::max(std::min(static_cast<int>(std::round(green * scalar)), 255), 0)),
                     static_cast<std::uint8_t>(std::max(std::min(static_cast<int>(std::round(blue * scalar)), 255), 0)),
                     static_cast<std::uint8_t>(std::max(std::min(static_cast<int>(std::round(alpha * scalar)), 255), 0)));
    }
};

