#pragma once

#include "entt/entt.hpp"
#include "opengl_functions.hxx"

#include "texture_type.hxx"

namespace sdk
{

class opengl_shader_system
{
public:
    static void init(entt::registry &registry)
    {
        auto view = registry.view<opengl_shader>();

        for (auto entity : view)
        {
            auto &shader = view.get<opengl_shader>(entity);
            create_shader_program(shader);
        }

        auto sprite_shader_view = registry.view<sprite>();

        for (auto entity : sprite_shader_view)
        {
            auto &ent_sprite = sprite_shader_view.get<sprite>(entity);
            create_shader_program(ent_sprite._shader);
        }
    }

private:
    static void create_shader_program(opengl_shader &shader)
    {
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
    }
};

} // namespace sdk
