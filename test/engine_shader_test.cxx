#include "opengl_functions.hxx"
#include "shader.hxx"
#include "vertex.hxx"

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <gtest/gtest.h>


TEST(TriangleTest, BasicInterpolation)
{
    constexpr int SCREEN_WIDTH = 800;
    constexpr int SCREEN_HEIGHT = 600;

    GL::init_sdl();

    SDL_Window *window = GL::create_window("", SCREEN_HEIGHT, SCREEN_WIDTH);

    SDL_GLContext context = GL::create_opengl_context(window);

    GL::load_opengl_functions();
    GL::is_opengl_version_supported();

    //glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Engine::Shader shader("vertex_shader.glsl", "fragment_shader.glsl");

    // Set up the triangle vertices
    std::array<Engine::Vertex, 3> vertices = {
        Engine::Vertex{1, 2},
        Engine::Vertex{3, 3},
        Engine::Vertex{2, 2}};

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
