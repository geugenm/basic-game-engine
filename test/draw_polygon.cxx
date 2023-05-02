#include "engine.hxx"

#include <gtest/gtest.h>

/// TODO: make it random
TEST(Polygon2DTest, DrawPolygonTest)
{
    const Position2D start(200, 230);
    const Position2D end(300, 1200);

    Polygon2D polygon(start, end, 8);

    Texture texture;
    texture.set_shape(BoundingBox(start, end));

    polygon.draw_on(texture, ColorRGB{0, 0, 255});

    std::filesystem::path path("test_polygon.ppm");
    PpmHandler handler(path, texture);
    handler.save();
}

/// TODO: make it random
TEST(Polygon2DTest, DrawMultiplePolygonsTest)
{
    Texture texture;
    texture.set_shape(BoundingBox(1100, 1100));

    for (size_t i = 0; i < 12; i++)
    {
        const Position2D start = {0, 0};
        const Position2D end(1000, 1000);
        const Position2D random(3, 12);
        Polygon2D polygon(start, end, static_cast<const size_t>(random.x));
        polygon.draw_on(texture, {0, 1, 255});
    }

    std::filesystem::path path("test_multiple_polygons.ppm");
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
    const Polygon2D& polygon2(polygon1);
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

TEST(Polygon2DTest, Triangulate)
{
    const BoundingBox bounding_box(1000, 1000);
    Texture texture;
    texture.set_shape(bounding_box);

    constexpr size_t angles_amount = 10;
    Polygon2D polygon(bounding_box, angles_amount);

    TriangulatedShape2D triangulated_shape2_d(polygon);
    triangulated_shape2_d.triangulate(texture, {155, 155, 0});

    texture.draw_mesh();
    PpmHandler handler("test_triangulation.ppm", texture);
    handler.save();
}

TEST(Polygon2DTest, Rasterize)
{
    const BoundingBox bounding_box(1000, 1000);
    Texture texture;
    texture.set_shape(bounding_box);

    constexpr size_t angles_amount = 3;
    Polygon2D polygon(bounding_box, angles_amount);

    polygon.fill(texture, {255, 0, 0});
    polygon.draw_on(texture, {0, 0, 255});

    texture.draw_mesh();
    PpmHandler handler("test_rasterization.ppm", texture);
    handler.save();
}

TEST(Polygon2DTest, Interpolate)
{
    Texture texture;
    texture.set_shape({1000, 1000});

    constexpr size_t angles_amount = 3;

    Polygon2D polygon(texture.get_shape(), angles_amount);

    polygon.interpolate(texture, {0, 0, 255}, {255, 0, 0});

    texture.draw_mesh();
    PpmHandler handler("test_interpolated.ppm", texture);
    handler.save();
}

TEST(Polygon2DTest, ApplyShader)
{
    PpmHandler handler1("leo.ppm");
    handler1.load();
    Texture leo = handler1.get_texture();

    Texture texture;
    texture.set_shape(leo.get_shape());

    constexpr size_t angles_amount = 3;

    Polygon2D polygon(texture.get_shape(), angles_amount);

    GFX::TestShader shader;
    polygon.apply_shader(texture, shader);

    texture.draw_mesh();
    PpmHandler handler("test_shader_applied.ppm", texture);
    handler.save();
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
