#include "render/shapes/line_2d.hxx"
#include "render/textures/ppm_handler.hxx"

#include <Tracy/tracy/Tracy.hpp>
#include <gtest/gtest.h>
#include <unistd.h>

TEST(Line2DTest, DrawRandomLinesTest)
{
    constexpr int num_lines = 500;

    const Position2D start = Position2D::generate_random(100, 730);
    const Position2D end = Position2D::generate_random(500, 1900);

    Line2D line(start, end);
    Texture texture;
    texture.set_shape({2000, 2000});

    for (int i = 0; i < num_lines; ++i)
    {
        sleep(1);
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
