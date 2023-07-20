#include "game_system.hxx"
#include "imgui_system.hxx"

namespace sdk
{

game_system::game_system(entt::registry &registry, const char *title,
                         const int &width, const int &height)
    : m_render_engine(registry, title, width, height)
{
    m_texture_system.test(registry);
    sdk::opengl_shader_initializer_system::init(registry);
    sdk::opengl_texture_system::init(registry);
    imgui.init(registry, m_render_engine._window_entity);
}

void game_system::update(entt::registry &registry)
{
    m_texture_system.update(registry, m_render_engine._window_entity);

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
            opengl_texture_system::destroy(registry);
        }

        switch (event.type)
        {
            case SDL_EVENT_FINGER_DOWN:
            case SDL_EVENT_FINGER_UP:
            case SDL_EVENT_FINGER_MOTION: {
                ImGuiIO &io = ImGui::GetIO();
                auto mouseX =
                    static_cast<int>(event.tfinger.x * io.DisplaySize.x);
                auto mouseY =
                    static_cast<int>(event.tfinger.y * io.DisplaySize.y);

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
            default:
                break;
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