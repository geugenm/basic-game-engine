#pragma once

#include "opengl_functions.hxx"
#include <abstract_engine.hxx>

#include <glad/glad.h>

namespace sdl_sdk
{

class engine : public sdk::engine<engine>
{
public:
    ~engine() override = default;

    template <typename... Args> void initialize_impl(const char *window_title, const int &height,
                         const int width, Args && ... args)
    {
        OpenGLWrapper::init_sdl();

        window_ =
            OpenGLWrapper::get_new_sdl_window(window_title, height, width);
        if (!window_)
        {
            SDL_Quit();
            throw std::invalid_argument("Failed to create SDL window");
        }

        context_ = OpenGLWrapper::get_new_sdl_gl_context(window_);
        if (!context_)
        {
            SDL_DestroyWindow(window_);
            SDL_Quit();
            throw std::invalid_argument("Failed to create OpenGL context");
        }

        OpenGLWrapper::init_opengl();
    }

    template <typename... Args> void render_impl(Args && ... args)
    {
        SDL_GL_SwapWindow(window_);
    }

    template <typename... Args> void destroy_impl(Args && ... args)
    {
        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

protected:
    SDL_Window *window_    = nullptr;
    SDL_GLContext context_ = nullptr;
};

sdl_sdk::engine * create_instance();

} // namespace sdl_sdk

template <> sdk::engine<sdl_sdk::engine> * sdk::create_instance() {
    return sdl_sdk::create_instance();
}