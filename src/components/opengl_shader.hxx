#pragma once

#include <filesystem>
#include <iostream>

#include <nlohmann/json.hpp>
#include <opengl_functions.hxx>

namespace sdk::suppl
{
[[nodiscard]] std::string
get_json_relative_path(const std::filesystem::path &full_path);
} // namespace sdk::suppl

namespace sdk
{
struct opengl_shader
{
    std::filesystem::path _vertex_source_path;
    std::filesystem::path _fragment_source_path;

    GLuint _program_id{};

    [[nodiscard]] GLint get_uniform_location(const GLchar *uniform_name) const;

    [[nodiscard]] static opengl_shader
    get_new_shader(const std::filesystem::path &vertex_source_path,
                   const std::filesystem::path &fragment_source_path);

    [[nodiscard]] static opengl_shader deserialize(
        const nlohmann::json &input_json,
        const std::filesystem::path &resources_path = "../assets/sprites");

    [[nodiscard]] nlohmann::json serialize() const;
};
} // namespace sdk