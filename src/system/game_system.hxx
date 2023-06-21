#pragma once

#include "audio_system.hxx"
#include "components/general_components.hxx"
#include "imgui_system.hxx"
#include "opengl_shader_initializer_system.hxx"
#include "sdl_render_system.hxx"
#include "texture_system.hxx"

#include "entt/entt.hpp"

namespace sdk
{

struct game_system
{
    [[no_unique_address]] opengl_texture_system texture_system{};
    [[no_unique_address]] opengl_shader_initializer_system shader_system;

    sdl_gl_engine render_engine;
    entt::entity game_state_entity;

    [[no_unique_address]] imgui_system imgui;
    // audio_system audio{"../assets/wav/car_on.WAV"};

    game_system(entt::registry &registry, const char *title, int height,
                int width)
        : render_engine(registry, title, height, width)
    {
        imgui.init(registry, render_engine._window_entity);
        texture_system.test(registry);
        sdk::opengl_shader_initializer_system::init(registry);
        texture_system.init_on(registry, render_engine._window_entity);

        game_states state = game_states::in_menu;
        game_state_entity = registry.create();
        registry.emplace<game_states>(game_state_entity, state);
    }

    void update(entt::registry &registry)
    {
        auto const &state = registry.get<game_states>(game_state_entity);

        if (state == game_states::played)
        {
            texture_system.update(registry, render_engine._window_entity);
        }

        if (state == game_states::paused)
        {
            texture_system.update(registry, render_engine._window_entity);
        }

        imgui.update(registry);
        render_engine.update(registry);
    }

    void handle_events(entt::registry &registry)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                render_engine.destroy(registry);
            }

            if (registry.view<game_states>().get<game_states>(
                    game_state_entity) == game_states::played)
            {
                texture_system.handle_events(registry, event);
            }

            if (registry.view<game_states>().get<game_states>(
                    game_state_entity) == game_states::exited)
            {
                render_engine.destroy(registry);
            }

            imgui_subsdk::process_event(event);
        }
    }
};

} // namespace sdk
