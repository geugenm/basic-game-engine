#include "color_rgb.hxx"

ColorRGB::ColorRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b) : red(r), green(g), blue(b)
{
}

bool ColorRGB::operator==(const ColorRGB &other) const
{
    return red == other.red && green == other.green && blue == other.blue;
}

bool ColorRGB::operator!=(const ColorRGB &other) const
{
    return !(*this == other);
}

ColorRGB ColorRGB::interpolate_linearly(const ColorRGB &first, const ColorRGB &second, double t)
{
    t = std::clamp(t, 0.0, 1.0);

    const auto r = static_cast<uint8_t>(first.red + (second.red - first.red) * t);
    const auto g = static_cast<uint8_t>(first.green + (second.green - first.green) * t);
    const auto b = static_cast<uint8_t>(first.blue + (second.blue - first.blue) * t);

    return {r, g, b};
}
