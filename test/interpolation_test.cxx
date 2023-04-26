#include "render/textures/ppm_handler.hxx"
#include "render/shapes/polygon_2d.hxx"

#include <gtest/gtest.h>

auto main(int argc, char **argv) -> int
{
    constexpr int width = 100;
    constexpr int height = 100;
    Texture texture;
    texture.set_shape({width, height});

    // Create a square polygon
    Position2D start(10, 10);
    Position2D end(90, 90);
    Polygon2D square({start, end}, 100);

    // Set the color to red
    ColorRGB color(255, 0, 0);

    // Draw the polygon on the texture
    square.triangulate();
    square.draw_on(texture, color);

    // Check that the corners of the square are red
//    EXPECT_EQ(texture.get_pixel(start), color);
//    EXPECT_EQ(texture.get_pixel({start.x, end.y}), color);
//    EXPECT_EQ(texture.get_pixel(end), color);
//    EXPECT_EQ(texture.get_pixel({end.x, start.y}), color);

    PpmHandler handler("123.ppm", texture);
    handler.save();
}
