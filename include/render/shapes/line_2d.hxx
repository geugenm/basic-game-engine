#pragma once

#include "render/abstract_renderer.hxx"
#include "render/textures/texture.hxx"

/**
 * A class for rendering 2D lines using the Bresenham algorithm.
 */
class LineRender2D {
public:
    /**
     * Constructs a new LineRender2D object with the given texture.
     * @param texture The texture to draw the lines on.
     */
    explicit LineRender2D(const Texture& texture)
        : texture_(std::make_unique<Texture>(texture)) { }

    /**
     * @brief Draws a 2D line using Bresenham's algorithm.
     *
     * @param start The starting position of the line.
     * @param end The ending position of the line.
     * @param color The color of the line.
     */
    void draw(const Position2D& start,
              const Position2D& end,
              const ColorRGB&   color) {
        int x0 = start.x;
        int y0 = start.y;
        int x1 = end.x;
        int y1 = end.y;

        const int dx = std::abs(x1 - x0);
        const int dy = std::abs(y1 - y0);
        const int sx = x0 < x1 ? 1 : -1;
        const int sy = y0 < y1 ? 1 : -1;

        int err = dx - dy;
        int e2;

        bool is_line_finished = false;
        while (is_line_finished == false) {
            texture_->set_pixel({ x0, y0 }, color);

            if (x0 == x1 && y0 == y1) {
                break;
            }

            e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }
    }

    /**
     * Returns the texture that the lines are being drawn on.
     * @return The texture object.
     */
    [[nodiscard]] const Texture& get_texture() const {
        return *texture_;
    }

private:
    std::unique_ptr<Texture> texture_;
};
