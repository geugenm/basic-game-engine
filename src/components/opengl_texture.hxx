#pragma once

#include <filesystem>
#include <iostream>

#include <nlohmann/json.hpp>
#include <opengl_functions.hxx>
#include <vector>

#include "opengl_shader.hxx"

namespace sdk
{
struct opengl_texture
{
    std::filesystem::path _image_path;

    GLuint _vertex_buffer_object{};
    GLuint _vertex_array_object{};
    GLuint _element_buffer_object{};

    GLuint _texture{};

    unsigned long _width{};
    unsigned long _height{};

    std::vector<GLfloat> _vertices;
    std::vector<GLuint> _indices;

    bool _need_generate_mipmaps = false;

    GLint _number{};
    
    void render() const
    {
        glBindTexture(GL_TEXTURE_2D, _texture);

        glBindVertexArray(_vertex_array_object);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void render_animated() const
    {
        glBindTexture(GL_TEXTURE_2D, _texture);
        glBindVertexArray(_vertex_array_object);

        glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_object);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(_vertices.size() * sizeof(GLfloat)),
            _vertices.data(), GL_DYNAMIC_DRAW);

        // Update vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void *)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void *)(3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void *)(6 * sizeof(float)));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    [[nodiscard]] static std::size_t
    get_tex_coordinates_index(const std::size_t &index)
    {
        return index * 8 + 6;
    }

    [[nodiscard]] static opengl_texture
    deserialize(const nlohmann::json &input_json)
    {
        return {
            ._image_path =
                resources_path.string() +
                input_json.value("image_path", "textures/missing_texture.png"),
            ._vertices = input_json.value("vertices", std::vector<GLfloat>{}),

            ._indices = input_json.value("indices", std::vector<GLuint>{}),
            ._need_generate_mipmaps =
                input_json.value("need_generate_mipmaps", false),
            ._number = input_json.value("number", 0),
        };
    }

    [[nodiscard]] nlohmann::json serialize() const
    {
        return {
            {"image_path", sdk::suppl::get_json_relative_path(_image_path)},
            {"vertices", _vertices},
            {"indices", _indices},
            {"need_generate_mipmaps", _need_generate_mipmaps},
        };
    }
};
} // namespace sdk