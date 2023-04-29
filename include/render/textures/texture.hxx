#pragma once

#include "render/colors/color_rgb.hxx"
#include "render/shapes/box.hxx"

#include <stdexcept>
#include <utility>
#include <vector>

class Texture final
{
  public:
    Texture() = default;

    explicit Texture(BoundingBox shape, const std::vector<ColorRGB> &pixels) : pixels_(pixels), shape_(std::move(shape))
    {
    }

    Texture(const Texture &other) : pixels_(other.pixels_), shape_(other.shape_)
    {
    }

    Texture(Texture &&other) noexcept : pixels_(std::move(other.pixels_)), shape_(std::move(other.shape_))
    {
    }

    Texture &operator=(const Texture &other)
    {
        if (this != &other)
        {
            pixels_ = other.pixels_;
            shape_ = other.shape_;
        }
        return *this;
    }

    Texture &operator=(Texture &&other) noexcept
    {
        if (this != &other)
        {
            pixels_ = std::move(other.pixels_);
            shape_ = std::move(other.shape_);
        }
        return *this;
    }

    ~Texture() = default;

    [[nodiscard]] const ColorRGB &get_pixel(const Position2D &position) const
    {
        if (!shape_.contains(position))
        {
            throw std::out_of_range("Error: Pixel position out of range.");
        }
        return pixels_[position.y * shape_.width + position.x];
    }

    void set_pixel(const Position2D &position, const ColorRGB &color)
    {
        if (!shape_.contains(position))
        {
            throw std::out_of_range("Error: Pixel position: " + position.string() +
                                    " out of range: " + shape_.string());
        }
        pixels_[position.y * shape_.width + position.x] = color;
    }

    void set_shape(const BoundingBox &shape)
    {
        shape_ = shape;
        resize_pixels();
    }

    [[nodiscard]] const BoundingBox &get_shape() const
    {
        return shape_;
    }

    void set_pixel_array(const std::vector<ColorRGB> &pixels)
    {
        resize_pixels();
        pixels_ = pixels;
    }

    [[nodiscard]] const std::vector<ColorRGB> &get_pixel_array() const
    {
        return pixels_;
    }

    [[nodiscard]] std::string string()
    {
        return "With the frame: " + shape_.string();
    }

  private:
    std::vector<ColorRGB> pixels_;
    BoundingBox shape_;

    void resize_pixels()
    {
        pixels_.resize(shape_.area());
    }
};
