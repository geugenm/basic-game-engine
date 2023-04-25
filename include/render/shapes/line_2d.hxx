#pragma once

#include "render/shapes/abstract_shape.hxx"
#include "render/textures/texture.hxx"

class Line2D final : public Shape2D
{
  public:
    Line2D(const Position2D &start, const Position2D &end) : start_(start), end_(end)
    {
        bounding_box_ = BoundingBox(start_, end_);
    }

    Line2D(const Line2D &other) : Shape2D(other), start_(other.start_), end_(other.end_), bounding_box_(other.bounding_box_)
    {
    }

    ~Line2D() override = default;

    void draw_on(Texture &texture, const ColorRGB &color) const override
    {
        int x0 = start_.x;
        int y0 = start_.y;
        int x1 = end_.x;
        int y1 = end_.y;

        const int dx = std::abs(x1 - x0);
        const int dy = std::abs(y1 - y0);

        int sx, sy;
        if (x0 < x1)
        {
            sx = 1;
        }
        else
        {
            sx = -1;
        }
        if (y0 < y1)
        {
            sy = 1;
        }
        else
        {
            sy = -1;
        }

        int err = dx - dy;
        int x = x0;
        int y = y0;

        while (true)
        {
            texture.set_pixel({x, y}, color);

            if (x == x1 && y == y1)
            {
                break;
            }

            int e2 = 2 * err;
            if (e2 > -dy)
            {
                err -= dy;
                x += sx;
            }
            if (e2 < dx)
            {
                err += dx;
                y += sy;
            }
        }
    }

    void draw_random(Texture &texture)
    {
        const Position2D start_container = start_;
        const Position2D end_container = end_;

        const Position2D range = {static_cast<int32_t>(bounding_box_.width - 1),
                                  static_cast<int32_t>(bounding_box_.height - 1)};
        start_ = Position2D::generate_random({0, 0}, range);
        end_ = Position2D::generate_random({0, 0}, range);

        draw_on(texture, ColorRGB::generate_random());
        start_ = start_container;
        end_ = end_container;
    }

    [[nodiscard]] std::unique_ptr<Shape2D> clone() const override
    {
        return std::make_unique<Line2D>(*this);
    }

    [[nodiscard]] std::string string() const override
    {
        return "Start: " + start_.string() + " End: " + end_.string();
    }

    [[nodiscard]] Vertices get_vertices() const override {
        Vertices result = {start_, end_};
        return result;
    }

    [[nodiscard]] const Position2D &get_start() const
    {
        return start_;
    }

    void set_start(const Position2D &start)
    {
        start_ = start;
    }

    [[nodiscard]] const Position2D &get_end() const
    {
        return end_;
    }

    void set_end(const Position2D &end)
    {
        end_ = end;
    }

    [[nodiscard]] Texture get_texture() override
    {
        return {};
    }

  private:
    Position2D start_;
    Position2D end_;

    BoundingBox bounding_box_;
};
