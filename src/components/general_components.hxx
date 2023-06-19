#pragma once

#include "../util/opengl/glad/glad.h"
#include <nlohmann/json.hpp>

#include <filesystem>
#include <glm/glm.hpp>
#include <vector>

namespace sdk
{
namespace util
{
[[nodiscard]] static nlohmann::json
get_file_json_content(std::filesystem::path file_path)
{
    static constexpr std::string_view properties_extension = ".json";

    file_path.replace_extension(properties_extension);

    if (!std::filesystem::exists(file_path))
    {
        throw std::invalid_argument("Json file '" + file_path.string() +
                                    "' was not found");
    }

    std::ifstream input_file(file_path);
    if (!input_file.is_open())
    {
        throw std::invalid_argument("Could not open file: " +
                                    file_path.string());
    }

    nlohmann::json json_content;

    input_file >> json_content;

    input_file.close();

    return json_content;
}
} // namespace util

struct opengl_shader
{
    std::filesystem::path _vertex_source_path;
    std::filesystem::path _fragment_source_path;

    GLuint _program_id{};

    [[nodiscard]] GLint get_uniform_location(const GLchar *uniform_name) const
    {
        const GLint uniform_location =
            glGetUniformLocation(_program_id, uniform_name);

        if (uniform_location == -1)
        {
            throw std::invalid_argument("Given uniform '" +
                                        std::string(uniform_name) +
                                        "' is not found.");
        }

        return uniform_location;
    }

    [[nodiscard]] static opengl_shader
    get_new_shader(const std::filesystem::path &vertex_source_path,
                   const std::filesystem::path &fragment_source_path)
    {
        opengl_shader shader{._vertex_source_path   = vertex_source_path,
                             ._fragment_source_path = fragment_source_path};

        if (!exists(shader._vertex_source_path))
        {
            throw std::invalid_argument("Vertex shader file '" +
                                        shader._vertex_source_path.string() +
                                        "' is not found.");
        }

        if (!exists(shader._fragment_source_path))
        {
            throw std::invalid_argument("Fragment shader file '" +
                                        shader._fragment_source_path.string() +
                                        "' is not found.");
        }

        const std::string vertex_source = shader._vertex_source_path.string();
        const std::string fragment_source =
            shader._fragment_source_path.string();

        GLuint vertex = opengl_subsdk::get_compiled_shader_from_file(
            GL_VERTEX_SHADER, vertex_source.data());
        GLuint fragment = opengl_subsdk::get_compiled_shader_from_file(
            GL_FRAGMENT_SHADER, fragment_source.data());

        shader._program_id = opengl_subsdk::get_new_program();

        opengl_subsdk::attach_shader(shader._program_id, vertex);
        opengl_subsdk::attach_shader(shader._program_id, fragment);

        opengl_subsdk::link_shader_program(shader._program_id);

        opengl_subsdk::delete_shader(vertex);
        opengl_subsdk::delete_shader(fragment);

        return shader;
    }
};

struct opengl_texture
{
    std::filesystem::path _image_path;

    GLuint _VBO{};
    GLuint _VAO{};
    GLuint _EBO{};

    GLuint _texture{};

    unsigned long _width{};
    unsigned long _height{};

    std::vector<GLfloat> _vertices;
    std::vector<GLuint> _indices;

    bool _need_generate_mipmaps = true;
    bool _needs_to_be_scaled    = true;

    GLint _number{};

    [[nodiscard]] float get_image_aspect_ratio() const
    {
        return static_cast<float>(_width) / static_cast<float>(_height);
    }
};

struct transform
{
    glm::vec2 _position{0.0f, -0.0f};
    float _current_rotation_angle = 0.0f;
};

struct sprite
{
    opengl_shader _shader;
    opengl_texture _texture;

    transform _transform;

    [[nodiscard]] static sprite get_sprite_from_file(
        const std::string_view &json_parameters_file_name,
        const std::filesystem::path &resources_path = "../resources/sprites")
    {

        const std::filesystem::path texture_path =
            resources_path / json_parameters_file_name;

        const nlohmann::json json_texture_properties =
            util::get_file_json_content(texture_path);

        if (json_texture_properties.empty())
        {
            throw std::invalid_argument("Empty texture properties file: " +
                                        texture_path.string());
        }

        sprite result{
            ._shader{
                ._vertex_source_path =
                    resources_path /
                    json_texture_properties["shader"]["vertex_source_path"],
                ._fragment_source_path =
                    resources_path /
                    json_texture_properties["shader"]["fragment_source_path"],
                ._program_id = opengl_subsdk::get_new_program(),
            },

            ._texture{
                ._image_path = resources_path /
                               json_texture_properties["texture"]["image_path"],
                ._vertices = json_texture_properties["texture"]["vertices"],

                ._indices = json_texture_properties["texture"]["indices"],
                ._need_generate_mipmaps =
                    json_texture_properties["texture"]["need_generate_mipmaps"],
                ._number = json_texture_properties["texture"]["number"],
            },
        };

        return result;
    }
};

enum class game_states
{
    paused,
    played,
    in_menu,
    exited
};
} // namespace sdk