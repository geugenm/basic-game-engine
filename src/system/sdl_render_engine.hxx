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

    [[nodiscard]] int get_width() const
    {
        int width;
        SDL_GetWindowSize(_window, &width, nullptr);
        return width;
    }

    [[nodiscard]] int get_height() const
    {
        int height;
        SDL_GetWindowSize(_window, nullptr, &height);
        return height;
    }
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

        glViewport(0, 0, width, height);
    }

    void update(entt::registry &registry)
    {
        static bool is_first_update = true;

        if (is_first_update)
        {
            is_first_update   = false;
            const auto entity = registry.create();
            registry.emplace<sdl_render_context>(entity, sdl_context);
        }

        if (!sdl_context.is_initialized())
        {
            throw sdk::engine_error("SDL engine is not initialized");
        }

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // TODO:
            //            const auto sdk_event = registry.create();
            //            registry.emplace<sdk::event>(
            //                sdk_event,
            //                sdl_subsdk::sdl_key_to_sdk_key(event.key.keysym.sym));

            imgui_subsdk::process_event(event);

            if (event.type == SDL_EVENT_QUIT)
            {
                destroy();
            }
        }

        SDL_GL_SwapWindow(sdl_context._window);
    }

    void destroy()
    {
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
