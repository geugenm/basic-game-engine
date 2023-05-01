#pragma once

#include <utility>

#include "abstract_shape.hxx"
#include "render/textures/texture.hxx"

class Line2D final : public Shape2D
{
  public:
    Line2D(Position2D start, Position2D end) : start_(std::move(start)), end_(std::move(end))
    {
        set_bounding_box(BoundingBox(start_, end_));
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
        Position2D current{start_.x, start_.y};

        while (current.x != end_.x || current.y != end_.y)
        {
            texture.set_pixel({current.x, current.y}, color);
            access_vertices().emplace_back(current.x, current.y);

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

    [[nodiscard]] std::string string() const override
    {
        return "Start: " + start_.string() + " End: " + end_.string();
    }

  private:
    Position2D start_;
    Position2D end_;
};
