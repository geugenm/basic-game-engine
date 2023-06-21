#include "game_system.hxx"

namespace sdk
{

game_system::game_system(entt::registry &registry, const char *title,
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

void game_system::update(entt::registry &registry)
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

void game_system::handle_events(entt::registry &registry)
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

bool game_system::is_initialized(entt::registry &registry) const
{
    return m_render_engine.is_initialized(registry);
}
} // namespace sdk