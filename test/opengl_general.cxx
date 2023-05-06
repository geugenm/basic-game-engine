#include <SDL.h>
#include <glad/glad.h>
#include <gtest/gtest.h>
#include <iostream>

constexpr uint16_t kWindowHeight       = 120;
constexpr uint16_t kWindowWidth        = 120;
constexpr uint16_t kOpenGLMajorVersion = 3;
constexpr uint16_t kOpenGLMinorVersion = 2;

bool InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

SDL_Window* CreateWindow()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, kOpenGLMajorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, kOpenGLMinorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    constexpr Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

    SDL_Window* window =
        SDL_CreateWindow("(opengl-compile-test) Test OPenGL SDL3 Window", kWindowHeight, kWindowWidth, flags);
    if (!window)
    {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
    }

    return window;
}

SDL_GLContext CreateOpenGLContext(SDL_Window* window)
{
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
    }

    return context;
}

bool LoadOpenGLFunctions()
{
    if (!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to load OpenGL functions with glad" << std::endl;
        return false;
    }
    return true;
}

bool IsOpenGLVersionSupported()
{
    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    if (major < kOpenGLMajorVersion ||
        (major == kOpenGLMajorVersion && minor < kOpenGLMinorVersion))
    {
        std::cerr << "Unsupported OpenGL version" << std::endl;
        return false;
    }

    return true;
}

TEST(OpenGLTest, GladInitialization)
{
    if (!InitSDL())
    {
        FAIL();
    }

    SDL_Window* window = CreateWindow();
    if (!window)
    {
        SDL_Quit();
        FAIL();
    }

    SDL_GLContext context = CreateOpenGLContext(window);
    if (!context)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        FAIL();
    }

    if (!LoadOpenGLFunctions() || !IsOpenGLVersionSupported())
    {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        FAIL();
    }

    // Fill the window with red color
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Enter the event loop to show the filled window
    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
        }

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers
        SDL_GL_SwapWindow(window);
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
