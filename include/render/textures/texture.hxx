#pragma once

#include "render/colors/color_rgb.hxx"

#include <vector>

#include <vector>

class Texture {
public:
    Texture() = default;

    Texture(std::size_t                  width,
            std::size_t                  height,
            const std::vector<ColorRGB>& pixels)
        : pixels_(pixels)
        , width_(width)
        , height_(height) { }

    [[nodiscard]] const std::vector<ColorRGB>& get_pixels() const {
        return pixels_;
    }

    [[nodiscard]] std::size_t get_width() const { return width_; }

    void set_width(std::size_t width) { width_ = width; }

    [[nodiscard]] std::size_t get_height() const { return height_; }

    void set_height(std::size_t height) { height_ = height; }

    [[nodiscard]] ColorRGB get_pixel(const size_t& x, const size_t& y) const {
        if (x >= width_ || y >= height_) {
            throw std::out_of_range("Pixel position out of range.");
        }
        return pixels_[y * width_ + x];
    }

    void set_pixel(const size_t& x, const size_t& y, const ColorRGB& color) {
        if (x >= width_ || y >= height_) {
            throw std::out_of_range("Pixel position out of range.");
        }
        pixels_[y * width_ + x] = color;
    }

    void set_dimensions(size_t width, size_t height) {
        if (width <= 0 || height <= 0) {
            throw std::invalid_argument(
                "Given 'height' or 'width' value is <= 0.");
        }
        pixels_.resize(width * height);
        width_  = width;
        height_ = height;
    }

    void set_pixels(const std::vector<ColorRGB>& pixels) {
        if (pixels.size() != width_ * height_) {
            throw std::invalid_argument(
                "Error: The number of pixels doesn't match the dimensions.");
        }

        pixels_ = pixels;
    }

private:
    std::vector<ColorRGB> pixels_;
    std::size_t           width_  = 0;
    std::size_t           height_ = 0;
};
