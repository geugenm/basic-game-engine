#pragma once

#include "glad/glad.h"

#include <render/picopng.hxx>

#include <fstream>
#include <stdexcept>
#include <valarray>
#include <vector>

namespace opengl_subsdk
{
class texture
{
public:
    explicit texture(const char *image_path) : image_path_(image_path)
    {
        if (image_path_ == nullptr)
        {
            throw std::invalid_argument("Given image path is empty");
        }
    }

    void initialize()
    {
        generate_buffers();
        bind_buffers();

        enable_attributes();
        glBindVertexArray(0);

        // Load and create a texture
        glGenTextures(1, &texture_);

        // All upcoming GL_TEXTURE_2D operations now have an effect on this
        // texture object
        glBindTexture(GL_TEXTURE_2D, texture_);

        set_texture_parameters();

        // Load image, create texture and generate mipmaps
        const auto png_data = get_png_data();

        if (width_ == 0 || height_ == 0)
        {
            throw std::invalid_argument("Given image bound are not determined. and Use get_png_data() "
                                        "before glTexImage2D.");
        }


        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, png_data.data());

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE0);
    }

    void render()
    {
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
    }

    void destroy()
    {
        glDeleteVertexArrays(1, &VAO);

        glDeleteBuffers(1, &VBO);

        glDeleteBuffers(1, &EBO);
    }

private:
    void generate_buffers()
    {
        glGenVertexArrays(1, &VAO);

        glGenBuffers(1, &VBO);

        glGenBuffers(1, &EBO);
    }

    void bind_buffers()
    {
        GLfloat vertices_[] = {
            // Positions          // Colors           // Texture Coords
            0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
            0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
            -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f  // Top Left
        };

        GLuint indices_[] = {
            // Note that we start from 0!
            0, 1, 3, // First Triangle
            1, 2, 3  // Second Triangle
        };

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_,
                     GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_,
                     GL_DYNAMIC_DRAW);
    }

    void enable_attributes()
    {
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
    }

    static void set_texture_parameters()
    {
        // Set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }


    [[nodiscard]] std::vector<unsigned char> get_png_data()
    {
        std::ifstream file(image_path_, std::ios::binary | std::ios::ate);

        if (!file.is_open())
        {
            throw std::invalid_argument("Failed to open PNG image");
        }

        const std::streamsize file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<unsigned char> buffer(static_cast<size_t>(file_size));
        if (!file.read(reinterpret_cast<char *>(buffer.data()), file_size))
        {
            throw std::invalid_argument("Failed to read PNG image");
        }

        std::vector<unsigned char> png_data;
        png_data.reserve(static_cast<size_t>(file_size));

        if (decodePNG(png_data, width_, height_, buffer.data(),
                      static_cast<size_t>(file_size)) != 0)
        {
            throw std::invalid_argument("Failed to decode PNG image");
        }

        if (png_data.empty())
        {
            throw std::invalid_argument("Empty PNG result");
        }

        return png_data;
    }

    const char *image_path_;

    GLuint VBO{};
    GLuint VAO{};
    GLuint EBO{};

    GLuint texture_{};

    unsigned long width_{};
    unsigned long height_{};
};
} // namespace opengl_subsdk