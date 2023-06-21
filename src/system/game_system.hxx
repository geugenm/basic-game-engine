#pragma once

#include "imgui_system.hxx"
#include "opengl_shader_initializer_system.hxx"
#include "sdl_render_system.hxx"
#include "texture_system.hxx"

#include <components/general_components.hxx>
#include <entt/entt.hpp>

namespace sdk
{

class game_system
{
public:
    explicit game_system(entt::registry &registry, const char *title,
                         const int &height, const int &width)
        : m_render_engine(registry, title, height, width),
          m_game_state_entity(registry.create())
    {
        imgui.init(registry, m_render_engine._window_entity);
        m_texture_system.test(registry);
        sdk::opengl_shader_initializer_system::init(registry);
        m_texture_system.init_on(registry, m_render_engine._window_entity);

        game_states state = game_states::in_menu;
        registry.emplace<game_states>(m_game_state_entity, state);
    }

    void update(entt::registry &registry)
    {
        auto const &state = registry.get<game_states>(m_game_state_entity);

        if (state == game_states::played)
        {
            m_texture_system.update(registry, m_render_engine._window_entity);
        }

        if (state == game_states::paused)
        {
            m_texture_system.update(registry, m_render_engine._window_entity);
        }

        imgui.update(registry);
        m_render_engine.update(registry);
    }

    void handle_events(entt::registry &registry)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                m_render_engine.destroy(registry);
            }

            if (registry.view<game_states>().get<game_states>(
                    m_game_state_entity) == game_states::played)
            {
                m_texture_system.handle_events(registry, event);
            }

            if (registry.view<game_states>().get<game_states>(
                    m_game_state_entity) == game_states::exited)
            {
                m_render_engine.destroy(registry);
            }

            imgui_subsdk::process_event(event);
        }
    }

    [[nodiscard]] bool is_initialized(entt::registry &registry) const
    {
        return m_render_engine.is_initialized(registry);
    }

private:
    [[no_unique_address]] opengl_texture_system m_texture_system{};
    [[no_unique_address]] opengl_shader_initializer_system m_shader_system;

    sdl_gl_engine m_render_engine;
    entt::entity m_game_state_entity;

    [[no_unique_address]] imgui_system imgui;
};

} // namespace sdk
