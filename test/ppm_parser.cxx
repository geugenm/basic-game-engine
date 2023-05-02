#include "engine.hxx"
#include <gtest/gtest.h>

class TextureFileTest : public testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        std::filesystem::create_directory("test_files");
    }

    static void TearDownTestSuite()
    {
        std::filesystem::remove_all("test_files");
    }

    void SetUp() override
    {
        std::ofstream out_file("test_files/test_texture.ppm", std::ios_base::binary);
        out_file << "P6\n"
                 << "2 2\n"
                 << "255\n"
                 << static_cast<char>(255) << static_cast<char>(0) << static_cast<char>(0) // red
                 << static_cast<char>(0) << static_cast<char>(255) << static_cast<char>(0) // green
                 << static_cast<char>(0) << static_cast<char>(0) << static_cast<char>(255) // blue
                 << static_cast<char>(255) << static_cast<char>(255)
                 << static_cast<char>(255);                                                // white
        out_file.close();
    }
};

TEST_F(TextureFileTest, LoadFile)
{
    auto texture_file = PpmHandler("test_files/test_texture.ppm");

    texture_file.load();

    const auto texture = texture_file.get_texture();

    EXPECT_EQ(texture.get_shape().width, 2);
    EXPECT_EQ(texture.get_shape().height, 2);

    const std::vector<ColorRGB> expected_pixels = {ColorRGB(255, 0, 0), ColorRGB(0, 255, 0),
                                                   ColorRGB(0, 0, 255), ColorRGB(255, 255, 255)};
    EXPECT_EQ(texture.get_pixel_array(), expected_pixels);
}

TEST_F(TextureFileTest, GetPixel)
{
    auto texture_file = PpmHandler("test_files/test_texture.ppm");

    texture_file.load();

    const auto texture = texture_file.get_texture();

    EXPECT_EQ(texture.get_pixel({0, 0}), ColorRGB(255, 0, 0));
    EXPECT_EQ(texture.get_pixel({1, 0}), ColorRGB(0, 255, 0));
    EXPECT_EQ(texture.get_pixel({0, 1}), ColorRGB(0, 0, 255));
    EXPECT_EQ(texture.get_pixel({1, 1}), ColorRGB(255, 255, 255));

    EXPECT_THROW(texture.get_pixel({2, 2}), std::out_of_range);
}

TEST_F(TextureFileTest, SetPixel)
{
    auto texture_file = PpmHandler("test_files/test_texture.ppm");

    texture_file.load();

    auto temp_texture = texture_file.get_texture();

    temp_texture.set_pixel({0, 0}, ColorRGB(0, 255, 0));
    EXPECT_EQ(temp_texture.get_pixel({0, 0}), ColorRGB(0, 255, 0));

    EXPECT_THROW(temp_texture.set_pixel({2, 2}, ColorRGB(255, 255, 255)), std::out_of_range);
}

TEST_F(TextureFileTest, SetDimensions)
{
    PpmHandler texture_file("test_files/test_texture.ppm");
    texture_file.load();

    auto texture_shape = texture_file.get_texture().get_shape();

    texture_shape.set_dimensions(1, 1);
    EXPECT_EQ(texture_shape.width, 1);
    EXPECT_EQ(texture_shape.height, 1);

    EXPECT_THROW(texture_shape.set_dimensions(0, 0), std::invalid_argument);
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
