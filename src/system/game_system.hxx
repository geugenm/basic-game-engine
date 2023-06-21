#pragma once

#include "imgui_system.hxx"
#include "opengl_shader_initializer_system.hxx"
#include "sdl_render_system.hxx"
#include "texture_system.hxx"

#include <entt/entt.hpp>
#include <general_components.hxx>

namespace sdk
{

class game_system
{
public:
    explicit game_system(entt::registry &registry, const char *title,
                         const int &height, const int &width);

    void update(entt::registry &registry);

    void handle_events(entt::registry &registry);

    [[nodiscard]] bool is_initialized(entt::registry &registry) const;

private:
    [[no_unique_address]] opengl_texture_system m_texture_system{};
    [[no_unique_address]] opengl_shader_initializer_system m_shader_system;

    sdl_gl_engine m_render_engine;
    entt::entity m_game_state_entity;

    [[no_unique_address]] imgui_system imgui;
};

} // namespace sdk
