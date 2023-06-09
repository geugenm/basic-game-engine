#pragma once

#include "imgui_wrapper.hxx"
#include "sdl_render_engine.hxx"

#include <entt/entt.hpp>

namespace sdk
{

struct imgui_window {
    const char * _name;
};

struct imgui_system
{
    explicit imgui_system(sdl_render_context sdl_render_context)
    {
        imgui_subsdk::init_imgui(sdl_render_context._window,
                                 sdl_render_context._context);
    }

    ~imgui_system() = default;

    void update(entt::registry &registry)
    {
        auto view = registry.view<sdk::imgui_window>();
        imgui_subsdk::new_frame();

        ImGui::ShowDemoWindow();

        imgui_subsdk::render();
    }
};

} // namespace sdk
