#include "engine.hxx"

auto main(int argc, char **argv) -> int
{
    constexpr int width = 1000;
    constexpr int height = 1000;
    Texture texture;
    texture.set_shape({width, height});

    Position2D start(100, 100);
    Position2D end(900, 900);
    Polygon2D square({start, end}, 5);

    ColorRGB color(255, 0, 0);

    square.draw_on(texture, color);

    TriangulatedShape2D triangulated_shape2_d(square);
    triangulated_shape2_d.triangulate();
    triangulated_shape2_d.draw_on(texture, {155, 155, 0});

    texture.draw_mesh();
    PpmHandler handler("test_interpolation.ppm", texture);
    handler.save();
}
