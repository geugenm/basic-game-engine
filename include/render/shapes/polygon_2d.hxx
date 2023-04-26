#pragma once

#include <utility>

#include "render/shapes/abstract_shape.hxx"
#include "render/shapes/line_2d.hxx"
#include "render/textures/texture.hxx"

class Polygon2D final : public Shape2D
{
  public:
    explicit Polygon2D(BoundingBox box, const size_t &sides_amount)
        : bounding_box_(std::move(box))
    {
        init_random(sides_amount);
    }

    explicit Polygon2D(BoundingBox box, Vertices vertices) : bounding_box_(std::move(box)), vertices_(std::move(vertices)) {

    }

    Polygon2D(const Position2D &start, const Position2D &end, const size_t &sides_amount)
    {
        bounding_box_ = BoundingBox(start, end);
        init_random(sides_amount);
    }

    Polygon2D(const Polygon2D &other) : Shape2D(other)
    {
        init_random(other.get_vertices().size());
    }

    ~Polygon2D() override = default;

    void draw_on(Texture &texture, const ColorRGB &color) const override
    {
        for (size_t i = 0; i < vertices_.size(); ++i)
        {
            const Position2D &p1 = vertices_[i];
            const Position2D &p2 = vertices_[(i + 1) % vertices_.size()];
            Line2D l(p1, p2);
            l.draw_on(texture, color);
        }
    }

    [[nodiscard]] std::unique_ptr<Shape2D> clone() const override
    {
        return std::make_unique<Polygon2D>(*this);
    }

    [[nodiscard]] std::string string() const override
    {
        return bounding_box_.string();
    }

    void add_vertex(const Position2D &position)
    {
        for (const auto &vertex : vertices_)
        {
            if (vertex == position)
            {
                return;
            }
        }
        vertices_.reserve(vertices_.size() + 1);
        vertices_.push_back(position);
    }

    [[nodiscard]] Vertices get_vertices() const override {
        return vertices_;
    }

  private:
    void init_random(const size_t & sides_amount)
    {
        double angle = 2.0 * M_PI / static_cast<double>(sides_amount);

        vertices_.resize(sides_amount);
        int32_t radius_x = static_cast<int32_t>(bounding_box_.width) / 2;
        int32_t radius_y = static_cast<int32_t>(bounding_box_.height) / 2;

        if (radius_x != 0)
        {
            radius_x -= 1;
        }

        if (radius_y != 0)
        {
            radius_y -= 1;
        }

        Position2D center = {radius_x, radius_y};

        for (size_t i = 0; i < sides_amount; ++i)
        {
            const double theta = angle * static_cast<double>(i);
            const double x = center.x + radius_x * std::cos(theta);
            const double y = center.y + radius_y * std::sin(theta);
            vertices_[i] = {static_cast<int>(x), static_cast<int>(y)};
        }
    }

    BoundingBox bounding_box_;

    Vertices vertices_;
};
