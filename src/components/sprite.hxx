#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>

#include "opengl_shader.hxx"
#include "opengl_texture.hxx"
#include "transform.hxx"

namespace sdk::suppl
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
} // namespace sdk::suppl

namespace sdk
{

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

    void save_to_file() const
    {
        const std::filesystem::path output_path =
            "../assets/sprites/" + _name + ".json";
        std::ofstream output_file(output_path);

        if (!output_file.is_open())
        {
            throw std::invalid_argument(
                "Could not write to file `" + output_path.string() +
                ".json`. Current searching path: " +
                std::filesystem::current_path().string());
        }

        const nlohmann::json output = serialize();
        output_file << output;
        output_file.close();
    }

    [[nodiscard]] static sprite get_sprite_from_file(
        const std::string_view &json_parameters_file_name,
        const std::filesystem::path &resources_path = "../assets/sprites")
    {

        const std::filesystem::path texture_path =
            resources_path / json_parameters_file_name;

        const nlohmann::json json_texture_properties =
            suppl::get_file_json_content(texture_path);

        if (json_texture_properties.empty())
        {
            throw std::invalid_argument("Empty texture properties file: " +
                                        texture_path.string());
        }

        auto result  = sprite::deserialize(json_texture_properties);
        result._name = json_parameters_file_name;

        return result;
    }

    [[nodiscard]] static sprite deserialize(const nlohmann::json &input_json)
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

} // namespace sdk
