#pragma once

#include "render/colors/color_rgb.hxx"

#include <stdexcept>
#include <vector>

class Texture
{
public:
    Texture() = default;

    Texture(std::size_t                  width,
            std::size_t                  height,
            const std::vector<ColorRGB>& pixels)
        : pixels_(pixels)
        , width_(width)
        , height_(height)
    {
    }

    [[nodiscard]] std::size_t get_width() const { return width_; }

    void set_width(std::size_t width)
    {
        if (width <= 0)
        {
            throw std::invalid_argument(
                "Error: Width must be a positive integer.");
        }
        width_ = width;
        resize_pixels();
    }

    [[nodiscard]] std::size_t get_height() const { return height_; }

    void set_height(std::size_t height)
    {
        if (height <= 0)
        {
            throw std::invalid_argument(
                "Error: Height must be a positive integer.");
        }
        height_ = height;
        resize_pixels();
    }

    [[nodiscard]] ColorRGB get_pixel(const size_t& x, const size_t& y) const
    {
        if (x >= width_ || y >= height_)
        {
            throw std::out_of_range("Error: Pixel position out of range.");
        }
        return pixels_[y * width_ + x];
    }

    void set_pixel(const size_t& x, const size_t& y, const ColorRGB& color)
    {
        if (x >= width_ || y >= height_)
        {
            throw std::out_of_range("Error: Pixel position out of range.");
        }
        pixels_[y * width_ + x] = color;
    }

    void set_dimensions(size_t width, size_t height)
    {
        if (width <= 0 || height <= 0)
        {
            throw std::invalid_argument(
                "Error: Width and height must be positive integers.");
        }
        width_  = width;
        height_ = height;
        resize_pixels();
    }

    void set_pixels(const std::vector<ColorRGB>& pixels)
    {
        if (pixels.size() != width_ * height_)
        {
            throw std::invalid_argument(
                "Error: The number of pixels doesn't match the dimensions.");
        }

        pixels_ = pixels;
    }

    [[nodiscard]] std::vector<ColorRGB> get_pixels() const { return pixels_; }

private:
    std::vector<ColorRGB> pixels_;
    std::size_t           width_  = 0;
    std::size_t           height_ = 0;

    void resize_pixels() { pixels_.resize(width_ * height_); }
};

;
