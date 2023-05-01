#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

#include <cstdint>
#include <random>

#pragma pack(push, 1)

struct ColorRGB
{
    std::uint8_t red = 0;
    std::uint8_t green = 0;
    std::uint8_t blue = 0;

    ColorRGB() = default;

    ColorRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b) : red(r), green(g), blue(b)
    {
    }

    ColorRGB(const ColorRGB &) = default;

    ColorRGB(ColorRGB &&) = default;

    ColorRGB &operator=(const ColorRGB &) = default;

    ColorRGB &operator=(ColorRGB &&) = default;

    bool operator==(const ColorRGB &other) const
    {
        return red == other.red && green == other.green && blue == other.blue;
    }

    bool operator!=(const ColorRGB &other) const
    {
        return !(*this == other);
    }

    [[nodiscard]] static ColorRGB interpolate_linearly(const ColorRGB& first, const ColorRGB& second, double t) {
        // Ensure t is within the valid range [0, 1]
        t = std::clamp(t, 0.0, 1.0);

        const auto r = static_cast<uint8_t>(first.red + (second.red - first.red) * t);
        const auto g = static_cast<uint8_t>(first.green + (second.green - first.green) * t);
        const auto b = static_cast<uint8_t>(first.blue + (second.blue - first.blue) * t);

        return {r, g, b};
    }
};

#pragma pack(pop)
