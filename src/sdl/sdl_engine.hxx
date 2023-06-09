#pragma once

#include "iengine.hxx"
#include "sdl_functions.hxx"

#include <glad/glad.h>

namespace sdl_subsdk
{

class engine
{
public:
    engine(const char *window_title, const int &height, const int width)
    {
        sdl_subsdk::init_sdl();

        window_ = sdl_subsdk::get_new_sdl_window(window_title, height, width);
        if (!window_)
        {
            SDL_Quit();
            throw sdk::engine_error("Failed to create SDL _window");
        }

        context_ = sdl_subsdk::get_new_sdl_gl_context(window_);
        if (!context_)
        {
            SDL_DestroyWindow(window_);
            SDL_Quit();
            throw sdk::engine_error("Failed to create OpenGL _context");
        }

        sdl_subsdk::init_opengl();
    }

    ~engine() = default;

    [[nodiscard]] bool is_initialized() const
    {
        return window_ != nullptr && context_ != nullptr;
    }

    [[nodiscard]] SDL_Window *get_window() const
    {
        if (!is_initialized())
        {
            LOG(WARNING)
                << "Trying to get the uninitialized _window or/and _context.";
        }
        return window_;
    }

    [[nodiscard]] SDL_GLContext get_context() const
    {
        if (!is_initialized())
        {
            LOG(WARNING)
                << "Trying to get the uninitialized _window or/and _context.";
        }
        return context_;
    }

    void render()
    {
        SDL_GL_SwapWindow(window_);
    }

    void destroy()
    {
        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

private:
    SDL_Window *window_    = nullptr;
    SDL_GLContext context_ = nullptr;
};

} // namespace sdl_subsdk