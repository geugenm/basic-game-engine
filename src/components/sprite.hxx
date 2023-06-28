#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>
#include <opengl_functions.hxx>
#include <sstream>

#include "SDL_rwops.h"
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

    std::string file_path_string = file_path;

    if (!file_path_string.empty() && file_path_string[0] == '/')
    {
        file_path_string.erase(0, 1); // remove the first character
    }

    // if (!std::filesystem::exists(file_path))
    // {
    //     throw std::invalid_argument("Json file '" + file_path.string() +
    //                                 "' was not found, looking for it in: " +
    //                                 std::filesystem::current_path().string());
    // }

    SDL_RWops *rw = SDL_RWFromFile(file_path_string.c_str(), "rb");
    if (rw == nullptr)
    {
        throw std::invalid_argument("Unable to open file: " + file_path_string +
                                    ", SDL says: " + SDL_GetError());
    }

    static constexpr uint16_t buffer_size = 2048;
    char buffer[buffer_size];
    const size_t read_count = SDL_RWread(rw, buffer, buffer_size - 1);
    if (read_count == 0)
    {
        throw std::invalid_argument(
            "Error reading from file: " + file_path_string +
            ", SDL says: " + SDL_GetError());
    }

    buffer[read_count] = '\0';

    SDL_RWclose(rw);

    return nlohmann::json::parse(buffer);
}

#include <streambuf>

struct membuf : public std::streambuf
{
    membuf() : std::streambuf(), buf(), buf_size(0) {}
    membuf(std::unique_ptr<char[]> buffer, size_t size)
        : std::streambuf(), buf(std::move(buffer)), buf_size(size)
    {
        char *beg_ptr = buf.get();
        char *end_ptr = beg_ptr + buf_size;
        setg(beg_ptr, beg_ptr, end_ptr);
        setp(beg_ptr, end_ptr);
    }
    membuf(membuf &&other)
    {
        setp(nullptr, nullptr);
        setg(nullptr, nullptr, nullptr);

        other.swap(*this);

        buf      = std::move(other.buf);
        buf_size = other.buf_size;

        other.buf_size = 0;
    }

    pos_type seekoff(off_type pos, std::ios_base::seekdir seek_dir,
                     std::ios_base::openmode) override
    {
        // TODO implement it in correct way
        if (seek_dir == std::ios_base::beg)
        {
            return 0 + pos;
        }
        else if (seek_dir == std::ios_base::end)
        {
            return buf_size + pos;
        }
        else
        {
            return egptr() - gptr();
        }
    }

    char *begin() const
    {
        return eback();
    }
    size_t size() const
    {
        return buf_size;
    }

private:
    std::unique_ptr<char[]> buf;
    size_t buf_size;
};

membuf load_file(std::string_view path);

[[nodiscard]] static std::string
get_file_content(std::filesystem::path file_path)
{
    std::string file_path_string = file_path;

    if (!file_path_string.empty() && file_path_string[0] == '/')
    {
        file_path_string.erase(0, 1); // remove the first character
    }

    SDL_RWops *rw = SDL_RWFromFile(file_path_string.c_str(), "rb");
    if (rw == nullptr)
    {
        throw std::invalid_argument("Unable to open file: " + file_path_string +
                                    ", SDL says: " + SDL_GetError());
    }

    static constexpr uint16_t buffer_size = 2048;
    char buffer[buffer_size];
    std::stringstream ss; // create a stringstream
    size_t read_count;
    while ((read_count = SDL_RWread(rw, buffer, buffer_size - 1)) > 0)
    {
        buffer[read_count] = '\0';
        ss << buffer; // add buffer content to the stringstream
    }

    if (SDL_RWclose(rw) != 0)
    {
        throw std::invalid_argument(
            "Error reading from file: " + file_path_string +
            ", SDL says: " + SDL_GetError());
    }

    std::string file_content = ss.str(); // get the string from the stringstream

    return file_content;
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

    [[nodiscard]] static sprite
    get_sprite_from_file(const std::string_view &json_parameters_file_name)
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
