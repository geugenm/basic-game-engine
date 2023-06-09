#pragma once

#include "engine_error.hxx"
#include "sdl_functions.hxx"

namespace sdk
{

struct sdl_engine
{
    SDL_Window *window    = nullptr;
    SDL_GLContext context = nullptr;

    sdl_engine(const char *window_title, const int &height, const int &width)
    {
        sdl_subsdk::init_sdl();

        window = sdl_subsdk::get_new_sdl_window(window_title, height, width);
        if (!window)
        {
            SDL_Quit();
            throw sdk::engine_error("Failed to create SDL window");
        }

        context = sdl_subsdk::get_new_sdl_gl_context(window);
        if (!context)
        {
            SDL_DestroyWindow(window);
            SDL_Quit();
            throw sdk::engine_error("Failed to create OpenGL context");
        }

        sdl_subsdk::init_opengl();
    }

    void render() const
    {
        SDL_GL_SwapWindow(window);
    }

    void update()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                destroy();
            }
        }
    }

    [[nodiscard]] bool is_initialized() const
    {
        return window != nullptr && context != nullptr;
    }

    void destroy()
    {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();

        window  = nullptr;
        context = nullptr;
    }

    ~sdl_engine()
    {
        destroy();
    }
};

} // namespace sdk
