#include "render/shapes/line_2d.hxx"
#include "render/textures/ppm_handler.hxx"
#include <gtest/gtest.h>

TEST(Line2DTest, DrawRandomLinesTest)
{
    constexpr int num_lines = 100;

    const Position2D start = Position2D::generate_random(200, 230);
    const Position2D end = Position2D::generate_random(300, 1200);

    Line2D line(start, end);
    Texture texture;
    texture.set_shape({400, 1300});

    for (int i = 0; i < num_lines; ++i)
    {
        line.draw_random(texture);
    }

    std::filesystem::path path("test_random_lines.ppm");
    PpmHandler handler(path, texture);
    handler.save();
}

auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
