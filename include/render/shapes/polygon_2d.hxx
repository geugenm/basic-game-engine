#pragma once

#include <filesystem>
#include <utility>

#include "render/shapes/abstract_shape.hxx"
#include "render/shapes/line_2d.hxx"
#include "render/textures/texture.hxx"

#include "render/textures/ppm_handler.hxx"

class Polygon2D final : public Shape2D
{
  public:
    explicit Polygon2D(const BoundingBox &box, const size_t &sides_amount)
    {
        set_bounding_box(box);
        init_random(sides_amount);
    }

    explicit Polygon2D(const BoundingBox &box, const Vertices &vertices)
    {
        set_bounding_box(box);
        set_vertices(vertices);
    }

    Polygon2D(const Position2D &start, const Position2D &end, const size_t &sides_amount)
    {
        set_bounding_box(BoundingBox(start, end));
        init_random(sides_amount);
    }

    Polygon2D(const Polygon2D &other) : Shape2D(other)
    {
        init_random(other.get_vertices().size());
    }

    Polygon2D(Polygon2D &&other) noexcept
    {
    }

    ~Polygon2D() override = default;

    void draw_on(Texture &texture, const ColorRGB &color) override
    {
        for (size_t i = 0; i < access_vertices().size(); ++i)
        {
            const Position2D &p1 = access_vertices()[i];
            const Position2D &p2 = access_vertices()[(i + 1) % access_vertices().size()];
            Line2D l(p1, p2);
            l.draw_on(texture, color);
        }
    }

    [[nodiscard]] std::string string() const override
    {
        return get_bounding_box().string();
    }

    void add_vertex(const Position2D &position)
    {
        for (const auto &vertex : access_vertices())
        {
            if (vertex == position)
            {
                return;
            }
        }
        access_vertices().reserve(access_vertices().size() + 1);
        access_vertices().push_back(position);
    }

  private:
    void init_random(const size_t &sides_amount)
    {
        double angle = 2.0 * M_PI / static_cast<double>(sides_amount);

        access_vertices().resize(sides_amount);
        int32_t radius_x = static_cast<int32_t>(access_bounding_box().width) / 2;
        int32_t radius_y = static_cast<int32_t>(access_bounding_box().height) / 2;

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
            access_vertices()[i] = {static_cast<int>(x), static_cast<int>(y)};
        }
    }
};
