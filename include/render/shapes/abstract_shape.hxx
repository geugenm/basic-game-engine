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

    [[nodiscard]] const Vertices & get_vertices() const {
        return vertices_;
    }

    [[nodiscard]] const BoundingBox & get_bounding_box() const {
        return bounding_box_;
    }

  protected:
    void set_bounding_box(const BoundingBox &boundingBox)
    {
        bounding_box_ = boundingBox;
    }

    void set_vertices(const Vertices &vertices)
    {
        vertices_ = vertices;
    }

    [[nodiscard]] Vertices & access_vertices() {
        return vertices_;
    }

    [[nodiscard]] BoundingBox & access_bounding_box() {
        return bounding_box_;
    }

  private:
    BoundingBox bounding_box_;

    Vertices vertices_;
};
