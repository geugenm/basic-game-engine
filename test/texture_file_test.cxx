#include <gtest/gtest.h>
#include "textures/texture_file.hxx"

TEST(TextureFile, Constructor) {
    std::filesystem::path file_path = "test_texture.pam";
    TextureFile texture_file(file_path);
    EXPECT_EQ(texture_file.get_path(), file_path);
}

TEST(TextureFile, Load) {
    std::filesystem::path file_path = "test_texture.pam";
    TextureFile texture_file(file_path);
    texture_file.load();
    EXPECT_EQ(texture_file.get_width(), 512);
    EXPECT_EQ(texture_file.get_height(), 512);
}


TEST(TextureFile, Save) {
    std::filesystem::path file_path = "test_texture.pam";
    TextureFile texture_file(file_path);
    texture_file.load();

    std::vector<Color> original_pixels = texture_file.get_pixels();
    size_t original_width = texture_file.get_width();
    size_t original_height = texture_file.get_height();

    std::filesystem::path save_path = "saved_texture.pam";
    texture_file.set_dimensions(256, 256);
    texture_file.set_pixels(std::vector<Color>(256 * 256, Color(255, 0, 0, 255)));
    texture_file.save(save_path);

    TextureFile saved_texture_file(save_path);
    saved_texture_file.load();

    EXPECT_EQ(saved_texture_file.get_width(), 256);
    EXPECT_EQ(saved_texture_file.get_height(), 256);
    EXPECT_EQ(saved_texture_file.get_pixels(), std::vector<Color>(256 * 256, Color(255, 0, 0, 255)));

    // Clean up saved file
    std::filesystem::remove(save_path);
}

auto main(int argc, char** argv) -> int {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
