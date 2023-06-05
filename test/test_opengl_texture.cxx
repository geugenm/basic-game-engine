#include <sdl_engine.hxx>

#include <complex>
#include <gtest/gtest.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sdl_engine.hxx"
#include "open_gl_shader.hxx"

#include "open_gl_shader.hxx"
#include "render/picopng.hxx"

namespace opengl_subsdk {
    class texture {
    public:
        explicit texture(const char * image_path) : image_path_(image_path) {
            if (image_path_ == nullptr) {
                throw std::invalid_argument("Given image path is empty");
            }
        }

        void init() {
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
            glGenVertexArrays(1, &VAO);

            glGenBuffers(1, &VBO);

            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                         GL_STATIC_DRAW);

            // Position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                                  nullptr);

            glEnableVertexAttribArray(0);

            // Color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                                  (GLvoid *)(3 * sizeof(GLfloat)));

            glEnableVertexAttribArray(1);

            // TexCoord attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                                  (GLvoid *)(6 * sizeof(GLfloat)));

            glEnableVertexAttribArray(2);

            glBindVertexArray(0);

            std::ifstream file(image_path_,
                               std::ios::binary | std::ios::ate);
            if (!file.is_open())
            {
                throw std::invalid_argument("Failed to open PNG image");
            }

            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            std::vector<unsigned char> buffer(static_cast<unsigned long>(size));
            if (!file.read(reinterpret_cast<char *>(buffer.data()), size))
            {
                throw std::runtime_error("Failed to read PNG image");
            }

            std::vector<unsigned char> png_data;
            unsigned long width, height;

            // Decode PNG data using picopng
            auto error = static_cast<unsigned int>(
                decodePNG(png_data, width, height, buffer.data(), buffer.size()));
            if (error != 0)
            {
                throw std::runtime_error("Failed to decode PNG image");
            }

            if (png_data.empty())
            {
                throw std::runtime_error("Empty png result");
            }

            // Load and create a texture
            glGenTextures(1, &texture_);

            // All upcoming GL_TEXTURE_2D operations now have an effect on this texture
            // object
            glBindTexture(GL_TEXTURE_2D, texture_);

            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                            GL_REPEAT); // set texture wrapping to GL_REPEAT (default
                                        // wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // Set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Load image, create texture and generate mipmaps

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, png_data.data());

            opengl_subsdk::unbind_texture(GL_TEXTURE_2D);
        }

        void render() {
            glActiveTexture(GL_TEXTURE0);

            glBindTexture(GL_TEXTURE_2D, texture_);

            glBindVertexArray(VAO);
        }

        void destroy() {
            glDeleteVertexArrays(1, &VAO);

            glDeleteBuffers(1, &VBO);

            glDeleteBuffers(1, &EBO);
        }
    private:
        const char * image_path_;

        GLuint VBO{};
        GLuint VAO{};
        GLuint EBO{};

        GLuint texture_{};

    };
}

TEST(SDLEngineTest, Init)
{
    const char *window_title        = "TestSDLEngine";
    constexpr int32_t window_height = 1000;
    constexpr int32_t window_width  = 1000;

    auto engine =
        new sdl_subsdk::engine(window_title, window_height, window_width);
    engine->initialize();

    auto shader = opengl_subsdk::shader("shaders/texture_vertex.glsl",
                                        "shaders/texture_fragment.glsl");

    auto texture = new opengl_subsdk::texture("textures/texture.png");
    texture->init();

    shader.use();

    auto transformLoc = shader.get_uniform_location("transform");

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


        texture->render();
        shader.use();

        GLint textureUniformLocation =
            glGetUniformLocation(shader.get_program_id(), "ourTexture");

        glUniform1i(textureUniformLocation,
                    0); // Set the texture uniform to use texture unit 0

        glm::mat4 transform = glm::mat4(1.0f);
        const float time    = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        transform = glm::scale(transform, glm::vec3(0.2f, 0.2f, 0.2f));
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform =
            glm::rotate(transform, time * 0.05f, glm::vec3(0.0f, 0.0f, 1.0f));

        // Get matrix's uniform location and set matrix
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
                           glm::value_ptr(transform));


        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);

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