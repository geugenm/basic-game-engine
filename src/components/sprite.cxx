#include "sprite.hxx"

#include <file_loading_stuff.hxx>

namespace sdk
{

void sprite::apply_transform(const glm::mat4 &transform) const
{
    glUseProgram(_shader._program_id);

    glUniformMatrix4fv(_shader.get_uniform_location("transform"), 1, GL_FALSE,
                       glm::value_ptr(transform));
    glUseProgram(0);
}

void sprite::render() const
{
    glUseProgram(_shader._program_id);
    auto target = static_cast<GLenum>(GL_TEXTURE0 + _texture._number);
    glActiveTexture(target);
    _texture.render();
    glUseProgram(0);
}

void sprite::render_animated() const
{
    glUseProgram(_shader._program_id);
    auto target = static_cast<GLenum>(GL_TEXTURE0 + _texture._number);
    glActiveTexture(target);
    _texture.render_animated();
    glUseProgram(0);
}

void sprite::save_to_file() const
{
    const std::filesystem::path output_path =
        "../assets/sprites/" + _name + ".json";
    std::ofstream output_file(output_path);

    if (!output_file.is_open())
    {
        throw std::invalid_argument("Could not write to file `" +
                                    output_path.string() +
                                    ".json`. Current searching path: " +
                                    std::filesystem::current_path().string());
    }

    const nlohmann::json output = serialize();
    output_file << output;
    output_file.close();
}

sprite
sprite::get_sprite_from_file(const std::string_view &json_parameters_file_name)
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

sprite sprite::deserialize(const nlohmann::json &input_json)
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

nlohmann::json sprite::serialize() const
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
} // namespace sdk