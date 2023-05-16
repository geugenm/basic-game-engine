#include <sdl_engine.hxx>

#include <gtest/gtest.h>

TEST(SDLEngineTest, Init)
{
    const char* window_title        = "TestSDLEngine";
    constexpr int32_t window_height = 1000;
    constexpr int32_t window_width  = 1000;
    SDL::Engine::Instance::instance().initialize(window_title, window_height, window_width);

    SDL::OpenGLShader shader;
    shader.initialize_impl("shaders/simple_vertex.glsl",
                           "shaders/simple_fragment.glsl",
                           {{0, "a_position"}});

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // bottom-left
        0.5f, -0.5f, 0.0f, // bottom-right
        0.0f,  0.5f, 0.0f  // top-center
    };


    SDL_Event event;
    while (true)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                goto cleanup;
            }
        }

        shader.use(vertices);

        SDL::Engine::Instance::instance().render();
    }

cleanup:
    SDL::Engine::Instance::instance().destroy();
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}