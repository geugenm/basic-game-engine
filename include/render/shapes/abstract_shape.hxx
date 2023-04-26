#pragma once

#include "render/colors/color_rgb.hxx"
#include "render/shapes/position_2d.hxx"
#include "render/textures/texture.hxx"

#include <memory>
#include <vector>

using Vertices = std::vector<Position2D>;

class Shape2D
{
  public:
    Shape2D() = default;

    virtual ~Shape2D() = default;

    virtual void draw_on(Texture &texture, const ColorRGB &color) = 0;

    [[nodiscard]] virtual std::unique_ptr<Shape2D> clone() const = 0;

    [[nodiscard]] virtual std::string string() const = 0;

    [[nodiscard]] virtual Vertices get_vertices() const = 0;

    [[nodiscard]] virtual Texture get_texture() {
        Texture texture;
        draw_on(texture, {0, 255, 255});
        return texture;
    }

    [[nodiscard]] virtual std::vector<Position2D> get_pixels() {
        return pixels_;
    }

  protected:
    void add_position(const Position2D & position) {
        pixels_.push_back(position);
    }
    std::vector<Position2D> pixels_;
};
