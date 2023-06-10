#pragma once

#include "render/picopng.hxx"

#include <entt/entt.hpp>
#include <opengl_functions.hxx>

#include "sdl_render_engine.hxx"

namespace sdk
{

struct opengl_texture
{
    const char *_image_path;

    GLuint _VBO{};
    GLuint _VAO{};
    GLuint _EBO{};

    GLuint _texture{};

    unsigned long _width{};
    unsigned long _height{};

    std::vector<GLfloat> _vertices;
    std::vector<GLfloat> _indices;

    bool _is_initialized = false;

    sdl_render_context _attached_context;

    [[nodiscard]] float image_aspect_ratio() const
    {
        return static_cast<float>(_width) / static_cast<float>(_height);
    }
};

struct opengl_texture_system
{
    void init_on(entt::registry &registry, const sdl_render_context &context)
    {
        auto view = registry.view<opengl_texture>();

        for (auto entity : view)
        {
            auto &texture = view.get<opengl_texture>(entity);
            initialize(texture, context);
        }
    }

    void update(entt::registry &registry)
    {
        registry.view<opengl_texture>().each(
            [this](auto entity, auto &texture) {
                if (texture._is_initialized)
                {
                    render(texture);
                }
            });
    }

private:
    void render(opengl_texture &texture)
    {
        glBindTexture(GL_TEXTURE_2D, texture._texture);

        glBindVertexArray(texture._VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
    }

    void destroy(opengl_texture &texture)
    {
        glDeleteVertexArrays(1, &texture._VAO);

        glDeleteBuffers(1, &texture._VBO);

        glDeleteBuffers(1, &texture._EBO);
    }

    void initialize(opengl_texture &texture, const sdl_render_context &context)
    {
        // Load image, create texture and generate mipmaps
        const auto png_data = get_png_data(texture);

        if (texture._width == 0 || texture._height == 0)
        {
            throw std::invalid_argument(
                "Given image bound are not determined. and Use get_png_data() "
                "before glTexImage2D.");
        }

        generate_buffers(texture);
        bind_buffers(texture, context);

        enable_attributes();
        glBindVertexArray(0);

        // Load and create a texture
        glGenTextures(1, &texture._texture);

        // All upcoming GL_TEXTURE_2D operations now have an effect on this
        // texture object
        glBindTexture(GL_TEXTURE_2D, texture._texture);

        set_texture_parameters();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture._width, texture._height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, png_data.data());

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        texture._is_initialized = true;
    }

    [[nodiscard]] static std::vector<unsigned char>
    get_png_data(opengl_texture &texture)
    {
        std::ifstream file(texture._image_path,
                           std::ios::binary | std::ios::ate);

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

        if (decodePNG(png_data, texture._width, texture._height, buffer.data(),
                      static_cast<size_t>(file_size), true) != 0)
        {
            throw std::invalid_argument("Failed to decode PNG image");
        }

        if (png_data.empty())
        {
            throw std::invalid_argument("Empty PNG result");
        }

        return png_data;
    }

    void generate_buffers(opengl_texture &texture)
    {
        glGenVertexArrays(1, &texture._VAO);

        glGenBuffers(1, &texture._VBO);

        glGenBuffers(1, &texture._EBO);
    }

    void bind_buffers(opengl_texture &texture, sdl_render_context sdl_context)
    {
        float image_aspect_ratio = static_cast<float>(texture._width) /
                                   static_cast<float>(texture._height);
        float window_aspect_ratio =
            static_cast<float>(sdl_context.get_width()) /
            static_cast<float>(sdl_context.get_height());

        float scale_x, scale_y;

        if (image_aspect_ratio > window_aspect_ratio)
        {
            scale_x = 1.0f;
            scale_y = window_aspect_ratio / image_aspect_ratio;
        }
        else
        {
            scale_x = image_aspect_ratio / window_aspect_ratio;
            scale_y = 1.0f;
        }

        texture._vertices = {
            // Positions          // Colors           // Texture Coords
            0.5f * scale_x,
            0.5f * scale_y,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f, // Top Right
            0.5f * scale_x,
            -0.5f * scale_y,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            1.0f,
            0.0f, // Bottom Right
            -0.5f * scale_x,
            -0.5f * scale_y,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            0.0f, // Bottom Left
            -0.5f * scale_x,
            0.5f * scale_y,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f // Top Left
        };

        texture._indices = {
            // Note that we start from 0.
            0, 1, 3, // First Triangle
            1, 2, 3  // Second Triangle
        };

        glBindVertexArray(texture._VAO);

        glBindBuffer(GL_ARRAY_BUFFER, texture._VBO);

        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(size(texture._vertices)),
                     texture._vertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texture._EBO);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(size(texture._indices)),
                     texture._indices.data(), GL_DYNAMIC_DRAW);
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
};

} // namespace sdk
