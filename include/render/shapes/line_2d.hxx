#pragma once

#include <utility>

#include "render/shapes/abstract_shape.hxx"
#include "render/textures/texture.hxx"

class Line2D final : public Shape2D
{
  public:
    Line2D(Position2D start, Position2D end) : start_(std::move(start)), end_(std::move(end))
    {
        bounding_box_ = BoundingBox(start_, end_);
    }

    Line2D(const Line2D &other)
        : Shape2D(other), start_(other.start_), end_(other.end_), bounding_box_(other.bounding_box_)
    {
    }

    ~Line2D() override = default;

    void draw_on(Texture &texture, const ColorRGB &color) override
    {
        const int delta_x = std::abs(end_.x - start_.x);
        const int delta_y = std::abs(end_.y - start_.y);

        int step_x, step_y;
        if (start_.x < end_.x)
        {
            step_x = 1;
        }
        else
        {
            step_x = -1;
        }
        if (start_.y < end_.y)
        {
            step_y = 1;
        }
        else
        {
            step_y = -1;
        }

        int error = delta_x - delta_y;
        Position2D current;

        while (current.x != end_.x || current.y != end_.y)
        {
            texture.set_pixel({current.x, current.y}, color);
            attached_pixels_.emplace_back(current.x, current.y);

            const int double_error = 2 * error;
            if (double_error > -delta_y)
            {
                error -= delta_y;
                current.x += step_x;
            }
            if (double_error < delta_x)
            {
                error += delta_x;
                current.y += step_y;
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

    [[nodiscard]] Vertices get_vertices() const override
    {
        Vertices result = {start_, end_};
        return result;
    }

    [[nodiscard]] BoundingBox get_bounding_box() const
    {
        return bounding_box_;
    }

  private:
    Position2D start_;
    Position2D end_;

    BoundingBox bounding_box_;

    std::vector<Position2D> attached_pixels_;
};
