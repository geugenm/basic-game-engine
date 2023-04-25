#pragma once

#include "render/colors/color_rgb.hxx"
#include "render/shapes/position_2d.hxx"

#include <memory>
#include <vector>

/**
 * @brief Abstract base class for all 2D shapes
 */
class Shape2D {
public:
    /**
     * @brief Default constructor
     */
    Shape2D() = default;

    /**
     * @brief Default destructor
     */
    virtual ~Shape2D() = default;

    /**
     * @brief Draws the shape onto the given canvas
     * @param canvas The 2D canvas to draw the shape onto
     * @param color The color to use when drawing the shape
     */
    virtual void draw(const ColorRGB& color) const = 0;

    /**
     * @brief Moves the shape by the given amount in the x and y directions
     * @param delta_x The amount to move the shape in the x direction
     * @param delta_y The amount to move the shape in the y direction
     */
    virtual void move(const Position2D& delta) = 0;

    /**
     * @brief Scales the shape by the given amount in the x and y directions
     * @param scale_x The amount to scale the shape in the x direction
     * @param scale_y The amount to scale the shape in the y direction
     */
    virtual void scale(double scale_x, double scale_y) = 0;

    /**
     * @brief Rotates the shape by the given angle (in degrees) around the given
     * point
     * @param angle The angle (in degrees) to rotate the shape by
     * @param center_x The x coordinate of the rotation center
     * @param center_y The y coordinate of the rotation center
     */
    virtual void rotate(double angle, const Position2D& center) = 0;

    /**
     * @brief Returns the area of the shape
     * @return The area of the shape
     */
    [[nodiscard]] virtual double area() const = 0;

    /**
     * @brief Returns the perimeter of the shape
     * @return The perimeter of the shape
     */
    [[nodiscard]] virtual double perimeter() const = 0;

    /**
     * @brief Returns a deep copy of the shape
     * @return A unique_ptr to a deep copy of the shape
     */
    [[nodiscard]] virtual std::unique_ptr<Shape2D> clone() const = 0;

    /**
     * @brief String representation of the shape
     * @return A string representation of the shape (its current parameters)
     */
    [[nodiscard]] virtual std::string string() const = 0;
};
