#pragma once

#include "render/colors/color_rgb.hxx"
#include "render/shapes/position_2d.hxx"
#include "render/textures/texture.hxx"

#include <memory>
#include <vector>

class Shape2D
{
  public:
    Shape2D() = default;

    virtual ~Shape2D() = default;

    virtual void draw_on(Texture &texture, const ColorRGB &color) const = 0;

    [[nodiscard]] virtual Texture get_texture() = 0;

    [[nodiscard]] virtual std::unique_ptr<Shape2D> clone() const = 0;

    [[nodiscard]] virtual std::string string() const = 0;
};
