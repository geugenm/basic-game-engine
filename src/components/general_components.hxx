#pragma once

#include "nlohmann/json_fwd.hpp"
#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <opengl_functions.hxx>

#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <ostream>
#include <vector>

#include <SDL3/SDL.h>

namespace sdk
{
struct sdl_render_context
{
    SDL_Window *_window    = nullptr;
    SDL_GLContext _context = nullptr;

    [[nodiscard]] bool is_initialized() const
    {
        return _window != nullptr && _context != nullptr;
    }

    [[nodiscard]] int get_width() const
    {
        int width;
        SDL_GetWindowSize(_window, &width, nullptr);
        return width;
    }

    [[nodiscard]] int get_height() const
    {
        int height;
        SDL_GetWindowSize(_window, nullptr, &height);
        return height;
    }
};

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
                                    "' was not found, looking for it in: " +
                                    std::filesystem::current_path().string());
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

[[nodiscard]] nlohmann::json glm_vec3_to_json(const glm::vec3 &vec);

[[nodiscard]] glm::vec3 glm_vec3_from_json(const nlohmann::json &j,
                                           const glm::vec3 &default_value = {
                                               0.f, 0.f, 0.f});

[[nodiscard]] std::string
get_json_relative_path(const std::filesystem::path &full_path);
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

    [[nodiscard]] static opengl_shader deserialize(
        const nlohmann::json &input_json,
        const std::filesystem::path &resources_path = "../assets/sprites")
    {
        return {
            ._vertex_source_path =
                resources_path /
                input_json.value("vertex_source_path", "shaders/missing.vert"),
            ._fragment_source_path =
                resources_path / input_json.value("fragment_source_path",
                                                  "shaders/missing.frag"),
            ._program_id = input_json.value("program_id", GLuint{}),
        };
    }

    [[nodiscard]] nlohmann::json serialize() const
    {
        return {{"vertex_source_path",
                 util::get_json_relative_path(_vertex_source_path)},
                {"fragment_source_path",
                 util::get_json_relative_path(_fragment_source_path)},
                {"program_id", _program_id}};
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

    bool _need_generate_mipmaps = false;

    GLint _number{};

    [[nodiscard]] float get_image_aspect_ratio() const
    {
        return static_cast<float>(_width) / static_cast<float>(_height);
    }

    void render() const
    {
        glBindTexture(GL_TEXTURE_2D, _texture);

        glBindVertexArray(_VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void render_animated() const
    {
        glBindTexture(GL_TEXTURE_2D, _texture);
        glBindVertexArray(_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(_vertices.size() * sizeof(GLfloat)),
            _vertices.data(), GL_DYNAMIC_DRAW);

        // Update vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              nullptr);
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

    [[nodiscard]] static opengl_texture deserialize(
        const nlohmann::json &input_json,
        const std::filesystem::path &resources_path = "../assets/sprites")
    {
        return {
            ._image_path =
                resources_path /
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
            {"image_path", util::get_json_relative_path(_image_path)},
            {"vertices", _vertices},
            {"indices", _indices},
            {"need_generate_mipmaps", _need_generate_mipmaps},
        };
    }
};

struct transform
{
    glm::vec3 _position{0.0f, 0.0f, 0.0f};
    glm::vec3 _scale{1.0f, 1.0f, 1.0f};
    float _current_rotation_angle = 0.0f;

    [[nodiscard]] static transform deserialize(
        const nlohmann::json &input_json,
        const std::filesystem::path &resources_path = "../assets/sprites")
    {

        return {
            ._position = util::glm_vec3_from_json(
                input_json.value("position", nlohmann::json::object())),
            ._scale = util::glm_vec3_from_json(
                input_json.value("scale", nlohmann::json::object()),
                {1.0f, 1.0f, 1.0f}),

            ._current_rotation_angle = input_json.value("rotation_angle", 0.0f),
        };
    }

    [[nodiscard]] nlohmann::json serialize() const
    {
        nlohmann::json result{
            {"position", nlohmann::json::object()},
            {"scale", nlohmann::json::object()},
            {"rotation_angle", _current_rotation_angle},
        };
        result["position"].merge_patch(util::glm_vec3_to_json(_position));
        result["scale"].merge_patch(util::glm_vec3_to_json(_scale));
        return result;
    }
};

struct sprite
{
    opengl_shader _shader;
    opengl_texture _texture;

    transform _transform;

    std::string _name;

    void apply_transform(const glm::mat4 &transform) const
    {
        glUseProgram(_shader._program_id);

        glUniformMatrix4fv(_shader.get_uniform_location("transform"), 1,
                           GL_FALSE, glm::value_ptr(transform));
        glUseProgram(0);
    }

    void render() const
    {
        glUseProgram(_shader._program_id);
        auto target = static_cast<GLenum>(GL_TEXTURE0 + _texture._number);
        glActiveTexture(target);
        _texture.render();
        glUseProgram(0);
    }

    void render_animated() const
    {
        glUseProgram(_shader._program_id);
        auto target = static_cast<GLenum>(GL_TEXTURE0 + _texture._number);
        glActiveTexture(target);
        _texture.render_animated();
        glUseProgram(0);
    }

    [[nodiscard]] static sprite get_sprite_from_file(
        const std::string_view &json_parameters_file_name,
        const std::filesystem::path &resources_path = "../assets/sprites")
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

        return sprite::deserialize(json_texture_properties);
    }

    [[nodiscard]] static sprite deserialize(
        const nlohmann::json &input_json,
        const std::filesystem::path &resources_path = "../assets/sprites")
    {
        return {
            ._shader = opengl_shader::deserialize(
                input_json.value("shader", nlohmann::json::object())),

            ._texture = opengl_texture::deserialize(
                input_json.value("texture", nlohmann::json::object())),

            ._transform = transform::deserialize(
                input_json.value("transform", nlohmann::json::object())),

            ._name = input_json.value("name", "null"),
        };
    }

    [[nodiscard]] nlohmann::json serialize() const
    {
        nlohmann::json result{
            {"shader", nlohmann::json::object()},
            {"texture", nlohmann::json::object()},
            {"transform", nlohmann::json::object()},
            {"json_path", _name},
        };
        result["shader"].merge_patch(_shader.serialize());
        result["texture"].merge_patch(_texture.serialize());
        result["transform"].merge_patch(_transform.serialize());
        return result;
    }
};

struct sprite_animation final
{
    std::size_t _current_frame{};
    std::size_t _rows{};
    std::size_t _columns{};

    [[nodiscard]] static sprite_animation
    create_new_animation(const std::size_t &rows, const std::size_t &cols)
    {
        if (rows == 0 || cols == 0)
        {
            throw std::invalid_argument(
                "Can't create sprite animation: `rows={}` or "
                "`cols={}` is 0");
        }

        sprite_animation new_animation{
            ._current_frame = 0,
            ._rows          = rows,
            ._columns       = cols,
        };

        return new_animation;
    }

    [[nodiscard]] bool is_properly_initialized() const
    {
        if (_rows == 0 || _columns == 0)
        {

            std::cout << "WARNING: sprite animation is not properly "
                         "initialized: `rows={}` or "
                         "`cols={}` is 0";
            return false;
        }

        return true;
    }
};

enum class game_states
{
    paused,
    played,
    in_menu,
    exited,
};
} // namespace sdk