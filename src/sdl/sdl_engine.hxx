#pragma once

#include "opengl_functions.hxx"
#include "sdl_shader.hxx"
#include <abstract_engine.hxx>

#include <glad/glad.h>

namespace SDL
{

class Engine : public ::Engine::Instance<Engine>
{
public:
    template <typename... Args>
    void initialize_impl(const char* window_title, const int& height, const int width,
                         Args&&... args)
    {
        if (!GL::init_sdl())
        {
            throw std::invalid_argument("Failed to init sdl");
        }

        window_ = GL::create_window(window_title, height, width);
        if (!window_)
        {
            SDL_Quit();
            throw std::invalid_argument("Failed to create SDL window");
        }

        context_ = GL::create_opengl_context(window_);
        if (!context_)
        {
            SDL_DestroyWindow(window_);
            SDL_Quit();
            throw std::invalid_argument("Failed to create OpenGL context");
        }

        if (!GL::load_opengl_functions() || !GL::is_opengl_version_supported())
        {
            SDL_GL_DeleteContext(context_);
            SDL_DestroyWindow(window_);
            SDL_Quit();
            throw std::invalid_argument("Failed to load opengl");
        }
    }

    template <typename... Args> void render_impl(Args&&... args)
    {
        SDL_GL_SwapWindow(window_);
        GL::listen_opengl_errors();
    }

    template <typename... Args> void destroy_impl(Args&&... args)
    {
        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

private:
    SDL_Window* window_    = nullptr;
    SDL_GLContext context_ = nullptr;
};

} // namespace SDL

template <>::Engine::Instance<SDL::Engine>* SDL::Engine::Instance::create_instance()
{
    return new SDL::Engine();
}