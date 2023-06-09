#pragma once

#include "sdl_render_engine.hxx"
#include "imgui_system.hxx"

#include "entt/entt.hpp"

namespace sdk
{

struct game_system
{
    sdl_gl_engine render_engine;
    imgui_system imgui;

    game_system(const char *title, int height, int width)
        : render_engine(title, height, width), imgui(render_engine.sdl_context)
    {
        // set up any other necessary variables for the system
    }

    void update(entt::registry &registry)
    {
        render_engine.update(registry);
        imgui.update(registry);
    }
};

} // namespace sdk
