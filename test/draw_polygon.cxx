#include "render/shapes/indexed_shape.hxx"
#include "render/shapes/polygon_2d.hxx"

#include <Tracy/tracy/Tracy.hpp>
#include <gtest/gtest.h>

TEST(Polygon2DTest, DrawPolygonTest)
{
    const Position2D start = Position2D::generate_random(200, 230);
    const Position2D end = Position2D::generate_random(300, 1200);

    Polygon2D polygon(start, end, 8);

    Texture texture;
    texture.set_shape(BoundingBox(start, end));

    polygon.draw_on(texture, ColorRGB{0, 0, 255});

    std::filesystem::path path("test_polygon.ppm");
    PpmHandler handler(path, texture);
    handler.save();
}

TEST(Polygon2DTest, DrawMultiplePolygonsTest)
{
    Texture texture;
    texture.set_shape(BoundingBox(1100, 1100));

    for (size_t i = 0; i < 12; i++)
    {
        const Position2D start = {0, 0};
        const Position2D end = Position2D::generate_random(1000, 1000);
        const Position2D random = Position2D::generate_random(3, 12);
        Polygon2D polygon(start, end, static_cast<const size_t>(random.x));
        polygon.draw_on(texture, ColorRGB::generate_random());
    }

    std::filesystem::path path("test_multiple_polygons.ppm");
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

TEST(Polygon2DTest, ConstructorWithBoundingBoxAndSidesAmount)
{
    BoundingBox box({0, 0}, {10, 10});
    Polygon2D polygon(box, 5);
    EXPECT_EQ(polygon.get_vertices().size(), 5);
}

TEST(Polygon2DTest, ConstructorWithBoundingBoxAndVertices)
{
    BoundingBox box({0, 0}, {10, 10});
    Vertices vertices = {{2, 2}, {8, 2}, {8, 8}, {2, 8}};
    Polygon2D polygon(box, vertices);
    EXPECT_EQ(polygon.get_vertices(), vertices);
}

TEST(Polygon2DTest, ConstructorWithStartEndPositionsAndSidesAmount)
{
    Position2D start(0, 0);
    Position2D end(10, 10);
    Polygon2D polygon(start, end, 4);
    EXPECT_EQ(polygon.get_vertices().size(), 4);
}

TEST(Polygon2DTest, CopyConstructor)
{
    const BoundingBox box({0, 0}, {10, 10});
    const Vertices vertices = {{2, 2}, {8, 2}, {8, 8}, {2, 8}};
    const Polygon2D polygon1(box, vertices);
    const Polygon2D polygon2(polygon1);
    EXPECT_EQ(polygon1.get_vertices(), polygon2.get_vertices());
}

TEST(Polygon2DTest, AddVertex)
{
    BoundingBox box({0, 0}, {10, 10});
    Polygon2D polygon(box, 3);
    polygon.add_vertex({5, 5});
    EXPECT_EQ(polygon.get_vertices().size(), 4);
    EXPECT_EQ(polygon.get_vertices().back(), Position2D(5, 5));
}

auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
