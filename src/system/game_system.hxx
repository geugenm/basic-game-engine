#pragma once

#include "sdl_render_engine.hxx"

namespace sdk
{

struct game_system
{
    sdl_engine render_engine;

    game_system(const char *title, int height, int width)
        : render_engine(title, height, width)
    {
        // set up any other necessary variables for the system
    }

    void update()
    {
        render_engine.render();

        render_engine.update();
    }
};

} // namespace sdk
