#pragma once

#include "imgui_wrapper.hxx"
#include "sdl_render_engine.hxx"

#include <entt/entt.hpp>

namespace sdk
{

struct imgui_window
{
    const char *_name;
};

struct imgui_system
{
    explicit imgui_system(const sdl_render_context &sdl_render_context)
    {
        imgui_subsdk::init_imgui(sdl_render_context._window,
                                 sdl_render_context._context);
    }

    ~imgui_system() = default;

    void update(entt::registry &registry)
    {
        imgui_subsdk::new_frame();
        // TODO: implement a normal event system
        //        auto view = registry.view<sdk::event>();
        //        imgui_subsdk::new_frame();
        //
        //        for (auto event : view) {
        //            auto const &sdk_event = view.get<sdk::event>(event);
        //            imgui_subsdk::process_event(sdk_event);
        //        }

        ImGui::ShowDemoWindow();

        imgui_subsdk::render();
    }
};

} // namespace sdk
