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
    entt::entity _window_entity;

    sdl_gl_engine(entt::registry &registry, const char *window_title,
                  const int &height, const int &width)
    {
        sdl_render_context sdl_context;
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

        _window_entity = registry.create();
        registry.emplace<sdl_render_context>(_window_entity, sdl_context);
    }

    [[nodiscard]] bool is_initialized(entt::registry &registry) const
    {
        auto view = registry.view<sdl_render_context>();
        return view.get<sdl_render_context>(_window_entity).is_initialized();
    }

    void update(entt::registry &registry) const
    {
        auto view         = registry.view<sdl_render_context>();
        auto &sdl_context = view.get<sdl_render_context>(_window_entity);

        if (!sdl_context.is_initialized())
        {
            throw sdk::engine_error("SDL engine is not initialized");
        }

        SDL_GL_SwapWindow(sdl_context._window);
    }

    void destroy(entt::registry &registry) const
    {
        auto view         = registry.view<sdl_render_context>();
        auto &sdl_context = view.get<sdl_render_context>(_window_entity);

        SDL_GL_DeleteContext(sdl_context._context);
        SDL_DestroyWindow(sdl_context._window);
        SDL_Quit();

        sdl_context._window  = nullptr;
        sdl_context._context = nullptr;
    }
};

} // namespace sdk
