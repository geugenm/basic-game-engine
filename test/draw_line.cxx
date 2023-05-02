#include "render/shapes/line_2d.hxx"
#include "render/textures/ppm_handler.hxx"

#include <Tracy/tracy/Tracy.hpp>
#include <gtest/gtest.h>
#include <unistd.h>

/// TODO: fix random
TEST(Line2DTest, DrawRandomLinesTest)
{
    constexpr int num_lines = 20;

    const Position2D start = Position2D::generate_random(0, 500);
    const Position2D end   = Position2D::generate_random(0, 500);

    Line2D line(start, end);
    Texture texture;
    texture.set_shape(line.get_bounding_box());

    for (int i = 0; i < num_lines; ++i)
    {
        sleep(1);
        line.draw_random(texture);
    }

    std::filesystem::path path("test_random_lines.ppm");
    PpmHandler handler(path, texture);
    handler.save();
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
