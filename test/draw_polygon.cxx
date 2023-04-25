#include "render/shapes/polygon_2d.hxx"
#include "render/textures/ppm_handler.hxx"
#include <gtest/gtest.h>

TEST(Polygon2DTest, DrawPolygonTest) {
    constexpr int num_lines = 100;

    const Position2D start = Position2D::generate_random(200, 230);
    const Position2D end   = Position2D::generate_random(300, 1200);

    Polygon2D polygon(start, end);

    polygon.draw(ColorRGB{0, 0, 255});

    std::filesystem::path path("test_polygon.ppm");
    PpmHandler            handler(path, polygon.get_texture());
    handler.save();
}

auto main(int argc, char** argv) -> int {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
