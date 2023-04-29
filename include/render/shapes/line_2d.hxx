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

    Line2D(const Line2D &other)
        : Shape2D(other), start_(other.start_), end_(other.end_), bounding_box_(other.bounding_box_)
    {
    }

    ~Line2D() override = default;

    void draw_on(Texture &texture, const ColorRGB &color) override
    {
        int start_x = start_.x;
        int start_y = start_.y;
        int end_x = end_.x;
        int end_y = end_.y;

        const int delta_x = std::abs(end_x - start_x);
        const int delta_y = std::abs(end_y - start_y);

        int step_x, step_y;
        if (start_x < end_x)
        {
            step_x = 1;
        }
        else
        {
            step_x = -1;
        }
        if (start_y < end_y)
        {
            step_y = 1;
        }
        else
        {
            step_y = -1;
        }

        int error = delta_x - delta_y;
        int current_x = start_x;
        int current_y = start_y;

        while (current_x != end_x || current_y != end_y)
        {
            texture.set_pixel({current_x, current_y}, color);

            int double_error = 2 * error;
            if (double_error > -delta_y)
            {
                error -= delta_y;
                current_x += step_x;
            }
            if (double_error < delta_x)
            {
                error += delta_x;
                current_y += step_y;
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

    [[nodiscard]] BoundingBox get_bounding_box() const
    {
        return bounding_box_;
    }

  private:
    Position2D start_;
    Position2D end_;

    BoundingBox bounding_box_;
};
