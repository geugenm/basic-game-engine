#include "render/textures/ppm_handler.hxx"
#include <gtest/gtest.h>

class TextureFileTest : public testing::Test
{
protected:
    void SetUp() override
    {
        std::filesystem::create_directory("test_files");

        std::ofstream out_file("test_files/test_texture.ppm",
                               std::ios_base::binary);
        out_file << "P6\n"
                 << "2 2\n"
                 << "255\n"
                 << static_cast<char>(255) << static_cast<char>(0)
                 << static_cast<char>(0)    // red
                 << static_cast<char>(0) << static_cast<char>(255)
                 << static_cast<char>(0)    // green
                 << static_cast<char>(0) << static_cast<char>(0)
                 << static_cast<char>(255)  // blue
                 << static_cast<char>(255) << static_cast<char>(255)
                 << static_cast<char>(255); // white
        out_file.close();
    }

    void TearDown() override { std::filesystem::remove_all("test_files"); }
};

TEST_F(TextureFileTest, LoadFile)
{
    PpmHandler texture_file("test_files/test_texture.ppm");
    texture_file.load();

    EXPECT_EQ(texture_file.get_width(), 2);
    EXPECT_EQ(texture_file.get_height(), 2);

    std::vector<ColorRGB> expected_pixels = { ColorRGB(255, 0, 0),
                                              ColorRGB(0, 255, 0),
                                              ColorRGB(0, 0, 255),
                                              ColorRGB(255, 255, 255) };
    EXPECT_EQ(texture_file.get_pixels(), expected_pixels);
}

TEST_F(TextureFileTest, GetPixel)
{
    PpmHandler texture_file("test_files/test_texture.ppm");
    texture_file.load();

    EXPECT_EQ(texture_file.get_pixel(0, 0), ColorRGB(255, 0, 0));
    EXPECT_EQ(texture_file.get_pixel(1, 0), ColorRGB(0, 255, 0));
    EXPECT_EQ(texture_file.get_pixel(0, 1), ColorRGB(0, 0, 255));
    EXPECT_EQ(texture_file.get_pixel(1, 1), ColorRGB(255, 255, 255));

    EXPECT_THROW(texture_file.get_pixel(2, 2), std::out_of_range);
}

TEST_F(TextureFileTest, SetPixel)
{
    PpmHandler texture_file("test_files/test_texture.ppm");
    texture_file.load();

    texture_file.set_pixel(0, 0, ColorRGB(0, 255, 0));
    EXPECT_EQ(texture_file.get_pixel(0, 0), ColorRGB(0, 255, 0));

    EXPECT_THROW(texture_file.set_pixel(2, 2, ColorRGB(255, 255, 255)),
                 std::out_of_range);
}

TEST_F(TextureFileTest, SetDimensions)
{
    PpmHandler texture_file("test_files/test_texture.ppm");
    texture_file.load();

    texture_file.set_dimensions(1, 1);
    EXPECT_EQ(texture_file.get_width(), 1);
    EXPECT_EQ(texture_file.get_height(), 1);

    EXPECT_THROW(texture_file.set_dimensions(0, 0), std::invalid_argument);
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
