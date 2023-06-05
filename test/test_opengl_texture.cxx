#include <sdl_engine.hxx>

#include <complex>
#include <gtest/gtest.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <open_gl_shader.hxx>
#include <opengl_texture.hxx>

TEST(SDLEngineTest, Init)
{
    static const char *window_title        = "TestSDLEngine";
    constexpr int32_t window_height = 1000;
    constexpr int32_t window_width  = 1000;

    auto engine =
        new sdl_subsdk::engine(window_title, window_height, window_width);
    engine->initialize();

    auto shader = opengl_subsdk::shader("shaders/texture_vertex.glsl",
                                        "shaders/texture_fragment.glsl");

    auto texture = new opengl_subsdk::texture("textures/texture.png");
    texture->initialize();

    const auto transformLoc = shader.get_uniform_location("transform");

    SDL_Event event;
    while (true)
    {
        glViewport(0, 0, window_width, window_height);

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                goto cleanup;
            }
        }

        shader.use();

        const GLint textureUniformLocation =
            glGetUniformLocation(shader.get_program_id(), "ourTexture");

        glUniform1i(textureUniformLocation,
                    0); // Set the texture uniform to use texture unit 0

        auto transform = glm::mat4(1.0f);
        const float time    = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        transform = glm::scale(transform, glm::vec3(0.2f, 0.2f, 0.2f));
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform =
            glm::rotate(transform, time * 0.05f, glm::vec3(0.0f, 0.0f, 1.0f));

        // Get matrix's uniform location and set matrix
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
                           glm::value_ptr(transform));

        glClear(GL_COLOR_BUFFER_BIT);

        texture->render();

        engine->render();
    }

cleanup:
    texture->destroy();

    engine->destroy();
}

auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}