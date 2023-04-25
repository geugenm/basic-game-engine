#pragma once

#include <utility>

#include "render/shapes/abstract_shape.hxx"
#include "render/shapes/line_2d.hxx"
#include "render/textures/texture.hxx"

class Polygon2D final : public Shape2D {
public:
    explicit Polygon2D(BoundingBox box)
        : bounding_box_(std::move(box)) {
            init(3);
    }

    Polygon2D(const Position2D& start, const Position2D& end) {
        bounding_box_ = BoundingBox(start, end);
        init(3);
    }

    Polygon2D(const Polygon2D& other)
        : Shape2D(other) {
        init(3);
    }

    ~Polygon2D() override = default;

    void draw_on(Texture& texture, const ColorRGB& color) const override {
        for (size_t i = 0; i < edges_amount_; ++i) {
            const Position2D& p1 = vertices_[i];
            const Position2D& p2 = vertices_[i + 1];
            Line2D            l(p1, p2);
            l.draw_on(texture, color);
        }

        const Position2D& p1 = vertices_[sides_amount_ - 1];
        const Position2D& p2 = vertices_[0];
        Line2D            l(p1, p2);
        l.draw_on(texture, color);
    }

    [[nodiscard]] std::unique_ptr<Shape2D> clone() const override {
        return std::make_unique<Polygon2D>(*this);
    }

    [[nodiscard]] std::string string() const override {
        return bounding_box_.string();
    }

    [[nodiscard]] Texture get_texture() override {
        return {};
    }

private:
    void init(const size_t& sides_amount) {
        sides_amount_ = sides_amount;
        edges_amount_ = sides_amount_ - 1;
        angle_        = 2.0 * M_PI / static_cast<double>(sides_amount_);

        vertices_.reserve(sides_amount_);
        radius_x_ = static_cast<double>(bounding_box_.width) / 2.0;
        radius_y_ = static_cast<double>(bounding_box_.height) / 2.0;
        center_   = { static_cast<int>(radius_x_) - 1, static_cast<int>(radius_y_) - 1 };

        for (size_t i = 0; i < sides_amount_; ++i) {
            const double theta = angle_ * static_cast<double>(i);
            const double x     = center_.x + radius_x_ * std::cos(theta);
            const double y     = center_.y + radius_y_ * std::sin(theta);
            vertices_[i]       = {static_cast<int>(x), static_cast<int>(y)};
        }
    }

    BoundingBox bounding_box_;

    size_t sides_amount_;
    double angle_;

    double radius_x_;
    double radius_y_;
    size_t edges_amount_;

    std::vector<Position2D> vertices_;
    Position2D              center_;
};
