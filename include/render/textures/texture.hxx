#pragma once

#include "render/colors/color_rgb.hxx"

#include <vector>

#include <vector>

class Texture {
public:
    Texture() = default;

    Texture(std::size_t width, std::size_t height, const std::vector<ColorRGB>& pixels)
        : pixels_(pixels), width_(width), height_(height)
    {}

    [[nodiscard]] const std::vector<ColorRGB>& get_pixels() const {
        return pixels_;
    }

    void set_pixels(const std::vector<ColorRGB>& pixels) {
        pixels_ = pixels;
    }

    [[nodiscard]] std::size_t get_width() const {
        return width_;
    }

    void set_width(std::size_t width) {
        width_ = width;
    }

    [[nodiscard]] std::size_t get_height() const {
        return height_;
    }

    void set_height(std::size_t height) {
        height_ = height;
    }

private:
    std::vector<ColorRGB> pixels_;
    std::size_t width_  = 0;
    std::size_t height_ = 0;
};

