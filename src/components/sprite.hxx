#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <nlohmann/json.hpp>
#include <opengl_functions.hxx>
#include <sstream>

#include "opengl_shader.hxx"
#include "opengl_texture.hxx"
#include "transform.hxx"

namespace sdk
{

struct sprite
{
    opengl_shader _shader;
    opengl_texture _texture;

    transform _transform;

    std::string _name;

    void apply_transform(const glm::mat4 &transform) const;

    void render() const;

    void render_animated() const;

    void save_to_file() const;

    [[nodiscard]] static sprite
    get_sprite_from_file(std::string_view json_parameters_file_name);

    [[nodiscard]] static sprite deserialize(const nlohmann::json &input_json);

    [[nodiscard]] nlohmann::json serialize() const;
};

} // namespace sdk
