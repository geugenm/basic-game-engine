#pragma once

#include "render/shapes/abstract_shape.hxx"
#include "render/textures/texture.hxx"

class Line2D : public Shape2D {
public:
    Line2D(const Position2D& start, const Position2D& end)
        : start_(start)
        , end_(end)
        , texture_(std::make_unique<Texture>()) {
        Canvas box(start, end);
        texture_->set_shape(box);
    }

    Line2D(const Line2D& other)
        : Shape2D(other)
        , start_(other.start_)
        , end_(other.end_)
        , texture_(other.texture_ ? std::make_unique<Texture>(*other.texture_) : nullptr) { }

    void draw(const ColorRGB& color) const override {
        int x0 = start_.x;
        int y0 = start_.y;
        int x1 = end_.x;
        int y1 = end_.y;

        const int dx = std::abs(x1 - x0);
        const int dy = std::abs(y1 - y0);

        int sx, sy;
        if (x0 < x1) {
            sx = 1;
        } else {
            sx = -1;
        }
        if (y0 < y1) {
            sy = 1;
        } else {
            sy = -1;
        }

        int err = dx - dy;
        int x   = x0;
        int y   = y0;

        while (true) {
            texture_->set_pixel({ x, y }, color);

            if (x == x1 && y == y1) {
                break;
            }

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
    }

    void draw_random() {
        const Position2D start_container = start_;
        const Position2D end_container   = end_;

        const Position2D range = { static_cast<int32_t>(texture_->get_shape().width - 1),
                                   static_cast<int32_t>(texture_->get_shape().height - 1) };
        start_                 = Position2D::generate_random({ 0, 0 }, range);
        end_                   = Position2D::generate_random({ 0, 0 }, range);

        draw(ColorRGB::generate_random());
        start_ = start_container;
        end_   = end_container;
    }

    void move(const Position2D& delta) override {
        start_ += delta;
        end_ += delta;
    }

    void scale(double scale_x, double scale_y) override {
        start_.scale(scale_x, scale_y);
        end_.scale(scale_x, scale_y);
    }

    void rotate(double angle, const Position2D& center) override {
        const double radians = angle * (M_PI / 180.0);

        start_.x -= center.x;
        start_.y -= center.y;
        end_.x -= center.x;
        end_.y -= center.y;

        const double new_start_x = start_.x * std::cos(radians) - start_.y * std::sin(radians);
        const double new_start_y = start_.x * std::sin(radians) + start_.y * std::cos(radians);
        const double new_end_x   = end_.x * std::cos(radians) - end_.y * std::sin(radians);
        const double new_end_y   = end_.x * std::sin(radians) + end_.y * std::cos(radians);

        start_.x = static_cast<int32_t>(new_start_x + center.x);
        start_.y = static_cast<int32_t>(new_start_y + center.y);
        end_.x   = static_cast<int32_t>(new_end_x + center.x);
        end_.y   = static_cast<int32_t>(new_end_y + center.y);
    }

    [[nodiscard]] double area() const override {
        return 0.0;
    }

    [[nodiscard]] double perimeter() const override {
        return std::sqrt(std::pow(end_.x - start_.x, 2) + std::pow(end_.y - start_.y, 2));
    }

    [[nodiscard]] std::unique_ptr<Shape2D> clone() const override {
        return std::make_unique<Line2D>(*this);
    }

    [[nodiscard]] std::string string() const override {
        return "Start: " + start_.string() + " End: " + end_.string() + " On the texture: " + texture_->string();
    }

    [[nodiscard]] const Position2D& get_start() const {
        return start_;
    }

    void set_start(const Position2D& start) {
        start_ = start;
    }

    [[nodiscard]] const Position2D& get_end() const {
        return end_;
    }

    void set_end(const Position2D& end) {
        end_ = end;
    }

    [[nodiscard]] const Texture& get_texture() const {
        return *texture_;
    }

    void set_texture(std::unique_ptr<Texture> texture) {
        if (texture == nullptr) {
            throw std::invalid_argument("Given texture is null");
        }
        texture_ = std::move(texture);
    }

private:
    Position2D start_;
    Position2D end_;

    std::unique_ptr<Texture> texture_;
};
