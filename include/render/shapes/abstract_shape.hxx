#pragma once

#include "render/colors/color_rgb.hxx"
#include "render/shapes/position_2d.hxx"

#include <memory>
#include <vector>

class Shape2D {
public:
    Shape2D() = default;

    virtual ~Shape2D() = default;

    virtual void draw(const ColorRGB& color) const = 0;

    virtual void move(const Position2D& delta) = 0;

    virtual void scale(double scale_x, double scale_y) = 0;

    virtual void rotate(double angle, const Position2D& center) = 0;

    [[nodiscard]] virtual double area() const = 0;

    [[nodiscard]] virtual double perimeter() const = 0;


    [[nodiscard]] virtual std::unique_ptr<Shape2D> clone() const = 0;


    [[nodiscard]] virtual std::string string() const = 0;
};
