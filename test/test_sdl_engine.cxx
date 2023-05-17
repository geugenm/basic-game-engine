#include <sdl_engine.hxx>

#include <complex>
#include <gtest/gtest.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TEST(SDLEngineTest, Init)
{
    const char* window_title        = "TestSDLEngine";
    constexpr int32_t window_height = 1000;
    constexpr int32_t window_width  = 1000;
    SDL::Engine::Instance::instance().initialize(window_title, window_height, window_width);

    SDL::SomeShader shader;
    shader.initialize_impl("shaders/texture_vertex.glsl", "shaders/texture_fragment.glsl");

    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        // Positions          // Colors           // Texture Coords
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // Top Left
    };
    GLuint indices[] = {
        // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    GL::listen_opengl_errors();

    glGenBuffers(1, &VBO);
    GL::listen_opengl_errors();

    glGenBuffers(1, &EBO);
    GL::listen_opengl_errors();

    glBindVertexArray(VAO);
    GL::listen_opengl_errors();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    GL::listen_opengl_errors();

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GL::listen_opengl_errors();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    GL::listen_opengl_errors();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    GL::listen_opengl_errors();

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    GL::listen_opengl_errors();

    glEnableVertexAttribArray(0);
    GL::listen_opengl_errors();

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (GLvoid*)(3 * sizeof(GLfloat)));
    GL::listen_opengl_errors();

    glEnableVertexAttribArray(1);
    GL::listen_opengl_errors();

    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (GLvoid*)(6 * sizeof(GLfloat)));
    GL::listen_opengl_errors();

    glEnableVertexAttribArray(2);
    GL::listen_opengl_errors();

    glBindVertexArray(0); // Unbind VAO
    GL::listen_opengl_errors();

    std::ifstream file("textures/texture.png", std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        throw std::invalid_argument("Failed to open PNG image");
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
    {
        throw std::runtime_error("Failed to read PNG image");
    }

    std::vector<unsigned char> png_data;
    unsigned long width, height;

    // Decode PNG data using picopng
    auto error =
        static_cast<unsigned int>(decodePNG(png_data, width, height, buffer.data(), buffer.size()));
    if (error != 0)
    {
        throw std::runtime_error("Failed to decode PNG image");
    }

    if (png_data.empty())
    {
        throw std::runtime_error("Empty png result");
    }

    // Load and create a texture
    GLuint texture;
    glGenTextures(1, &texture);
    GL::listen_opengl_errors();

    glBindTexture(
        GL_TEXTURE_2D,
        texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
    GL::listen_opengl_errors();

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GL::listen_opengl_errors();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GL::listen_opengl_errors();

    // Load image, create texture and generate mipmaps

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 png_data.data());
    GL::listen_opengl_errors();

    glBindTexture(GL_TEXTURE_2D,
                  0); // Unbind texture when done, so we won't accidentily mess up our texture.
    GL::listen_opengl_errors();

    glm::mat4 trans;
    trans = glm::rotate(trans, 90.0f, glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    auto transformLoc =
        static_cast<GLint>(glGetUniformLocation(shader.get_shader_program_id(), "transform"));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

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

        glBindTexture(GL_TEXTURE_2D, texture);
        GL::listen_opengl_errors();

        shader.render();

        glBindVertexArray(VAO);
        GL::listen_opengl_errors();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        GL::listen_opengl_errors();

        glBindVertexArray(0);
        GL::listen_opengl_errors();

        SDL::Engine::Instance::instance().render();
    }

cleanup:
    glDeleteVertexArrays(1, &VAO);
    GL::listen_opengl_errors();

    glDeleteBuffers(1, &VBO);
    GL::listen_opengl_errors();

    glDeleteBuffers(1, &EBO);
    GL::listen_opengl_errors();

    SDL::Engine::instance().destroy();
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}