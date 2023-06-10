#pragma once

#include "audio_system.hxx"
#include "imgui_system.hxx"
#include "opengl_shader_system.hxx"
#include "opengl_texture.hxx"
#include "sdl_render_engine.hxx"

#include "entt/entt.hpp"

namespace sdk
{

struct game_system
{
    [[no_unique_address]] opengl_texture_system texture_system;
    [[no_unique_address]] opengl_shader_system shader_system;

    sdl_gl_engine render_engine;

    [[no_unique_address]] imgui_system imgui;
    audio_system audio{"../resources/wav/car_on.WAV"};

    game_system(entt::registry &registry, const char *title, int height,
                int width)
        : render_engine(title, height, width), imgui(render_engine.sdl_context)
    {
        opengl_subsdk::enable_debug_mode();
        texture_system.test(registry);
        shader_system.init(registry);
        texture_system.init_on(registry, render_engine.sdl_context);
        shader_system.update(registry);
    }

    void update(entt::registry &registry)
    {
        render_engine.update(registry);
        shader_system.update(registry);
        texture_system.update(registry);
        imgui.update(registry);
    }
};

} // namespace sdk
