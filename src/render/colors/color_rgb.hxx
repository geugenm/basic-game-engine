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

    ColorRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b);

    ColorRGB(const ColorRGB &) = default;

    ColorRGB(ColorRGB &&) = default;

    ColorRGB &operator=(const ColorRGB &) = default;

    ColorRGB &operator=(ColorRGB &&) = default;

    bool operator==(const ColorRGB &other) const;

    bool operator!=(const ColorRGB &other) const;

    [[nodiscard]] static ColorRGB interpolate_linearly(const ColorRGB& first, const ColorRGB& second, double t);
};

#pragma pack(pop)
