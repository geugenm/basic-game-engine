#include "render/picopng.hxx"
#include "sdl_engine.hxx"

#include <gtest/gtest.h>

#include <complex>
#include <fstream>

class TextureLoaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        SDL::Engine::instance().initialize("Test texture", 1280, 720);
    }

    void TearDown() override {
        SDL::Engine::instance().destroy();
    }

    SDL_Window* window = nullptr;
    SDL_GLContext gl_context = nullptr;
};

TEST_F(TextureLoaderTest, LoadTexture) {
    std::ifstream file("textures/texture.png", std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        FAIL() << "Failed to open PNG image";
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        FAIL() << "Failed to read PNG image";
    }

    std::vector<unsigned char> png_data;
    unsigned long width, height;

    // Decode PNG data using picopng
    unsigned error = decodePNG(png_data, width, height, buffer.data(), buffer.size());
    if (error != 0) {
        FAIL() << "Failed to decode PNG image";
    }

    // Create texture from PNG data
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, png_data.data());

    glBindTexture(GL_TEXTURE_2D, 0);

    // Check if texture was created successfully
    EXPECT_NE(texture, 0u);

    GLfloat vertices[] = {
        // Позиции          // Цвета             // Текстурные координаты
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Верхний левый
    };

    glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    GLuint VAO;
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

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

        SDL::Engine::Instance::instance().render();
    }

    cleanup:
    // Clean up
    glDeleteTextures(1, &texture);
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}