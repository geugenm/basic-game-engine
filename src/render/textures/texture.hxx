#pragma once

#include "../colors/color_rgb.hxx"
#include "../shapes/bounding_box.hxx"

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
        return pixels_[static_cast<size_t>(position.y) * shape_.width + static_cast<size_t>(position.x)];
    }

    void set_pixel(const Position2D &position, const ColorRGB &color)
    {
        if (!shape_.contains(position))
        {
            throw std::out_of_range("Error: Pixel position: " + position.string() +
                                    " out of range: " + shape_.string());
        }
        pixels_[static_cast<size_t>(position.y) * shape_.width + static_cast<size_t>(position.x)] = color;
    }

    void draw_mesh()
    {
        const auto &shape = get_shape();

        constexpr float kMeshDensity = 0.02f;
        const auto step = static_cast<size_t>((float)std::max(shape.width, shape.height) * kMeshDensity);

        // Draw horizontal lines
        for (std::size_t y = 0; y < shape.height; y += step)
        {
            for (std::size_t x = 0; x < shape.width; ++x)
            {
                set_pixel({static_cast<int32_t>(x), static_cast<int32_t>(y)}, {0, 255, 255});
            }
        }

        // Draw vertical lines
        for (std::size_t x = 0; x < shape.width; x += step)
        {
            for (std::size_t y = 0; y < shape.height; ++y)
            {
                set_pixel({static_cast<int32_t>(x), static_cast<int32_t>(y)}, {0, 255, 255});
            }
        }
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
