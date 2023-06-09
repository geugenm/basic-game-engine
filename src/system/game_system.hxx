#pragma once

#include "audio_system.hxx"
#include "imgui_system.hxx"
#include "sdl_render_engine.hxx"

#include "entt/entt.hpp"

namespace sdk
{

struct game_system
{
    sdl_gl_engine render_engine;
    [[no_unique_address]] imgui_system imgui;
    audio_system audio{"wav/car_on.WAV"};

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
