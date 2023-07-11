#include "game_system.hxx"
#include "imgui_system.hxx"

namespace sdk
{

game_system::game_system(entt::registry &registry, const char *title,
                         const int &width, const int &height)
    : m_render_engine(registry, title, width, height),
      m_game_state_entity(registry.create())
{
    m_texture_system.test(registry);
    sdk::opengl_shader_initializer_system::init(registry);
    m_texture_system.init_on(registry, m_render_engine._window_entity);
    imgui.init(registry, m_render_engine._window_entity);

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
                m_game_state_entity) == game_states::exited)
        {
            m_render_engine.destroy(registry);
        }

        switch (event.type)
        {
            case SDL_EVENT_FINGER_DOWN:
            case SDL_EVENT_FINGER_UP:
            case SDL_EVENT_FINGER_MOTION: {
                // Convert the touch coordinates to ImGui coordinates
                ImGuiIO &io = ImGui::GetIO();
                int mouseX  = event.tfinger.x * io.DisplaySize.x;
                int mouseY  = event.tfinger.y * io.DisplaySize.y;

                // Pass the touch event to ImGui
                if (event.type == SDL_EVENT_FINGER_DOWN)
                {
                    io.MouseDown[0] = true;
                }
                else if (event.type == SDL_EVENT_FINGER_UP)
                {
                    io.MouseDown[0] = false;
                }

                io.MousePos = ImVec2((float)mouseX, (float)mouseY);
                break;
            }
        }

        imgui_subsdk::process_event(event);
        imgui.handle_events(event, registry);
    }
}

bool game_system::is_initialized(entt::registry &registry) const
{
    return m_render_engine.is_initialized(registry);
}
} // namespace sdk