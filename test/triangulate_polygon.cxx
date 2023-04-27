#include "render/shapes/polygon_2d.hxx"
#include "render/textures/ppm_handler.hxx"

#include <Tracy/tracy/Tracy.hpp>

auto main(int argc, char **argv) -> int
{
    constexpr int width = 100;
    constexpr int height = 100;
    Texture texture;
    texture.set_shape({width, height});

    Position2D start(10, 10);
    Position2D end(90, 90);
    Polygon2D square({start, end}, 5);

    ColorRGB color(255, 0, 0);

    square.triangulate();
    square.draw_on(texture, color);

    PpmHandler handler("test_interpolation.ppm", texture);
    handler.save();
}
