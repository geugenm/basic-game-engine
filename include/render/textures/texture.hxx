#pragma once

#include "render/colors/color_rgb.hxx"
#include "render/shapes/box.hxx"

#include <stdexcept>
#include <vector>

class Texture {
public:
    Texture() = default;

    Texture(const Box2D& shape, const std::vector<ColorRGB>& pixels)
        : pixels_(pixels)
        , shape_(shape) { }

    [[nodiscard]] const ColorRGB& get_pixel(const Position2D& position) const {
        if (shape_.contains(position) == false) {
            throw std::out_of_range("Error: Pixel position out of range.");
        }
        return pixels_[position.y * shape_.width + position.x];
    }

    void set_pixel(const Position2D& position, const ColorRGB& color) {
        if (shape_.contains(position) == false) {
            throw std::out_of_range("Error: Pixel position out of range.");
        }
        pixels_[position.y * shape_.width + position.x] = color;
    }

    void set_shape(const Box2D& shape) {
        shape_ = shape;
    }

    [[nodiscard]] const Box2D& get_shape() const {
        return shape_;
    }

    void set_pixel_array(const std::vector<ColorRGB>& pixels) {
        resize_pixels();
        pixels_ = pixels;
    }

    [[nodiscard]] const std::vector<ColorRGB>& get_pixel_array() const {
        return pixels_;
    }

private:
    std::vector<ColorRGB> pixels_;
    Box2D                 shape_;

    void resize_pixels() {
        pixels_.resize(shape_.area());
    }
};

;
