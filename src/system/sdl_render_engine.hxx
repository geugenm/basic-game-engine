#pragma once

#include "engine_error.hxx"
#include "sdl_functions.hxx"

#include <entt/entt.hpp>

namespace sdk
{

struct sdl_render_context
{
    SDL_Window *_window    = nullptr;
    SDL_GLContext _context = nullptr;

    [[nodiscard]] bool is_initialized() const
    {
        return _window != nullptr && _context != nullptr;
    }
};

struct event_component
{
    event _event;
};

struct sdl_gl_engine
{
    sdl_render_context sdl_context;

    sdl_gl_engine(const char *window_title, const int &height, const int &width)
    {
        sdl_subsdk::init_sdl();

        sdl_context._window =
            sdl_subsdk::get_new_sdl_window(window_title, height, width);
        if (!sdl_context._window)
        {
            SDL_Quit();
            throw sdk::engine_error("Failed to create SDL _window");
        }

        sdl_context._context =
            sdl_subsdk::get_new_sdl_gl_context(sdl_context._window);
        if (!sdl_context._context)
        {
            SDL_DestroyWindow(sdl_context._window);
            SDL_Quit();
            throw sdk::engine_error("Failed to create OpenGL _context");
        }

        sdl_subsdk::init_opengl();
    }

    void update(entt::registry &registry)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            const auto sdk_event = registry.create();
            registry.emplace<event_component>(
                sdk_event,
                sdl_subsdk::sdl_key_to_sdk_key(event.key.keysym.sym));
            if (event.type == SDL_EVENT_QUIT)
            {
                destroy();
            }
        }

        if (!sdl_context.is_initialized())
        {
            throw sdk::engine_error("SDL engine is not initialized");
        }

        SDL_GL_SwapWindow(sdl_context._window);
    }

    void destroy()
    {
        if (!sdl_context.is_initialized())
        {
            return;
        }

        SDL_GL_DeleteContext(sdl_context._context);
        SDL_DestroyWindow(sdl_context._window);
        SDL_Quit();

        sdl_context._window  = nullptr;
        sdl_context._context = nullptr;
    }

    ~sdl_gl_engine()
    {
        destroy();
    }
};

} // namespace sdk
