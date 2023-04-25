/**
 * @file sdl_compilation.cxx
 * @brief Contains compilation verifying tests for SDL library.
 */

#include <SDL3/SDL.h>
#include <gtest/gtest.h>
#include <iostream>

/**
 * @brief Tests the SDL version.
 *
 * Checks that the compiled version of SDL matches the linked version.
 */
TEST(SDLTest, SDLVersion)
{
    SDL_version compiled_version = {0, 0, 0};
    SDL_version linked_version = {0, 0, 0};

    SDL_VERSION(&compiled_version)
    SDL_GetVersion(&linked_version);

    auto [compiled_major, compiled_minor, compiled_patch] = compiled_version;
    auto [linked_major, linked_minor, linked_patch] = linked_version;

    std::ostringstream compiled_version_str, linked_version_str;

    compiled_version_str << static_cast<int>(compiled_major) << "." << static_cast<int>(compiled_minor) << "."
                         << static_cast<int>(compiled_patch);
    linked_version_str << static_cast<int>(linked_major) << "." << static_cast<int>(linked_minor) << "."
                       << static_cast<int>(linked_patch);

    EXPECT_EQ(compiled_version_str.str(), linked_version_str.str());
}

/**
 * @brief Tests the SDL initialization.
 *
 * Checks that SDL initializes correctly by creating a window and renderer,
 * and then destroying them.
 */
TEST(SDLTest, SDLInitialization)
{
    constexpr uint16_t kWindowHeight = 120;
    constexpr uint16_t kWindowWidth = 120;

    ASSERT_EQ(SDL_Init(SDL_INIT_VIDEO), 0) << "SDL_Init Error: " << SDL_GetError();

    SDL_Window *window = SDL_CreateWindow("(sdl-compile-test) Test SDL3 Window", kWindowHeight, kWindowWidth, 0);
    ASSERT_NE(window, nullptr) << "SDL_CreateWindow Error: " << SDL_GetError();

    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    ASSERT_NE(renderer, nullptr) << "SDL_CreateRenderer Error: " << SDL_GetError();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
