#include "render/shapes/polygon_2d.hxx"

auto main(int argc, char **argv) -> int
{
    constexpr int width = 1000;
    constexpr int height = 1000;
    Texture texture;
    texture.set_shape({width, height});

    Position2D start(100, 100);
    Position2D end(900, 900);
    Polygon2D square({start, end}, 2);

    ColorRGB color(255, 0, 0);

    square.draw_on(texture, color);

    texture.draw_mesh();
    PpmHandler handler("test_interpolation.ppm", texture);
    handler.save();
}
