#include <gtest/gtest.h>
#include <glad/glad.h>
#include <SDL.h>
#include <iostream>

TEST(OpenGLTest, GladInitialization) {
    constexpr uint16_t kWindowHeight = 120;
    constexpr uint16_t kWindowWidth  = 120;

    // Initialize SDL
    ASSERT_EQ(SDL_Init(SDL_INIT_VIDEO), 0) << "Failed to initialize SDL: " << SDL_GetError();

    // Set up a window and a context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow("(sdl-compile-test) Test SDL3 Window", kWindowHeight, kWindowWidth, SDL_WINDOW_OPENGL);
    ASSERT_NE(window, nullptr) << "Failed to create SDL window: " << SDL_GetError();

    // Create an OpenGL context
    SDL_GLContext context = SDL_GL_CreateContext(window);
    ASSERT_NE(context, nullptr) << "Failed to create OpenGL context: " << SDL_GetError();

    // Load OpenGL functions using glad
    int gladStatus = gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress);
    ASSERT_EQ(gladStatus, 1) << "Failed to load OpenGL functions with glad";

    // Check the OpenGL version
    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    ASSERT_GE(major, 3) << "OpenGL major version is less than 3";
    if (major == 3) {
        ASSERT_GE(minor, 2) << "OpenGL minor version is less than 2";
    }

    // Clean up
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}



auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
