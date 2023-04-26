#include "render/shapes/polygon_2d.hxx"
#include "render/shapes/indexed_shape.hxx"
#include "render/textures/ppm_handler.hxx"

#include <gtest/gtest.h>


TEST(Polygon2DTest, DrawPolygonTest)
{
    const Position2D start = Position2D::generate_random(200, 230);
    const Position2D end = Position2D::generate_random(300, 1200);

    Polygon2D polygon(start, end, 6);

    Texture texture;
    texture.set_shape(BoundingBox(start, end));

    polygon.draw_on(texture, ColorRGB{0, 0, 255});

    std::filesystem::path path("test_polygon.ppm");
    PpmHandler handler(path, texture);
    handler.save();
}

TEST(Polygon2DTest, DrawMultiplePolygonsTest)
{
    const Position2D start = Position2D::generate_random(0, 0);
    const Position2D end = Position2D::generate_random(100, 100);

    Polygon2D polygon(start, end, 3);

    Texture texture;
    texture.set_shape(BoundingBox(500, 500));

    polygon.add_vertex({0, 0});
    polygon.add_vertex({200, 200});
    polygon.draw_on(texture, ColorRGB{0, 0, 255});

    std::filesystem::path path("test_polygon_additional_vertex.ppm");
    PpmHandler handler(path, texture);
    handler.save();
}

TEST(Polygon2DTest, DrawIndexedShape)
{
    const Position2D start = {200, 400};
    const Position2D end = {300, 400};

    IndexedShape indexed_shape;

    Texture texture;
    texture.set_shape(BoundingBox(600, 600));

    Polygon2D polygon(start, end, 0);
    polygon.add_vertex({0, 0});
    polygon.add_vertex({0, 100});
    polygon.add_vertex({100, 100});

    indexed_shape.add_2d_shape(polygon);

    Polygon2D polygon2(start, end, 0);

    polygon2.add_vertex({0, 0});
    polygon2.add_vertex({100, 0});
    polygon2.add_vertex({100, 100});


    const int rect_size = 50;

    for (int y = 0; y < texture.get_shape().height; y += rect_size)
    {
        for (int x = 0; x < texture.get_shape().width; x += rect_size)
        {
            Polygon2D rect({x, y}, {x + rect_size, y + rect_size}, 0);
            rect.add_vertex({0, 0});
            rect.add_vertex({0, rect_size});
            rect.add_vertex({rect_size, rect_size});
            rect.add_vertex({rect_size, 0});

            indexed_shape.add_2d_shape(rect);
        }
    }

    indexed_shape.add_2d_shape(polygon2);

    indexed_shape.draw_on(texture, {0, 255, 255});

    std::filesystem::path path("test_polygon_additional_vertex_indexed.ppm");
    PpmHandler handler(path, texture);
    handler.save();
}

auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
