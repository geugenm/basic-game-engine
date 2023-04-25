#include "render/shapes/line_2d.hxx"
#include "render/textures/ppm_handler.hxx"
#include <gtest/gtest.h>

TEST(Line2DTest, CopyConstructorTest) {
    Position2D start(0, 0);
    Position2D end(5, 5);
    Line2D     line(start, end);
    line.scale(2, 2);

    Line2D copy(line);

    EXPECT_EQ(copy.perimeter(), line.perimeter());
    EXPECT_EQ(copy.area(), line.area());
    EXPECT_EQ(copy.get_start().x, line.get_start().x);
    EXPECT_EQ(copy.get_start().y, line.get_start().y);
    EXPECT_EQ(copy.get_end().x, line.get_end().x);
    EXPECT_EQ(copy.get_end().y, line.get_end().y);
}

TEST(Line2DTest, MoveTest) {
    Position2D start(0, 0);
    Position2D end(5, 5);
    Line2D     line(start, end);

    line.move({ 2, 2 });

    EXPECT_EQ(line.get_start().x, start.x + 2);
    EXPECT_EQ(line.get_start().y, start.y + 2);
    EXPECT_EQ(line.get_end().x, end.x + 2);
    EXPECT_EQ(line.get_end().y, end.y + 2);
}

TEST(Line2DTest, ScaleTest) {
    Position2D start(0, 0);
    Position2D end(5, 5);
    Line2D     line(start, end);

    line.scale(2, 2);

    EXPECT_EQ(line.get_start().x, start.x * 2);
    EXPECT_EQ(line.get_start().y, start.y * 2);
    EXPECT_EQ(line.get_end().x, end.x * 2);
    EXPECT_EQ(line.get_end().y, end.y * 2);
}

TEST(Line2DTest, PerimeterTest) {
    Position2D start(0, 0);
    Position2D end(5, 5);
    Line2D     line(start, end);

    double expected_perimeter = std::sqrt(std::pow(end.x - start.x, 2) + std::pow(end.y - start.y, 2));

    EXPECT_EQ(line.perimeter(), expected_perimeter);
}

TEST(Line2DTest, DrawRandomLinesTest) {
    constexpr int num_lines = 100;

    const Position2D start = Position2D::generate_random(2, 23);
    const Position2D end   = Position2D::generate_random(3, 12);

    Line2D line(start, end);
    line.draw(ColorRGB(0, 255, 255));

    std::filesystem::path path("testx");
    PpmHandler handler(path, line.get_texture());

//    for (int i = 0; i < num_lines; ++i) {
//        line.draw_random();
//    }
}

auto main(int argc, char** argv) -> int {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
