#pragma once

#include <opengl_functions.hxx>

#include <entt/entt.hpp>

#include "opengl_shader.hxx"
#include <general_components.hxx>

namespace sdk
{

class opengl_shader_initializer_system
{
public:
    static void init(entt::registry &registry)
    {
        auto view = registry.view<opengl_shader>();

        for (auto entity : view)
        {
            auto &shader = view.get<opengl_shader>(entity);
            shader       = opengl_shader::get_new_shader(
                shader._vertex_source_path, shader._fragment_source_path);
        }

        auto sprite_shader_view = registry.view<sprite>();

        for (auto entity : sprite_shader_view)
        {
            auto &ent_sprite   = sprite_shader_view.get<sprite>(entity);
            ent_sprite._shader = opengl_shader::get_new_shader(
                ent_sprite._shader._vertex_source_path,
                ent_sprite._shader._fragment_source_path);
        }
    }
};

} // namespace sdk
