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
    void initialize_impl(const char *window_title, const int &height,
                         const int width)
    {
        if (!OpenGLWrapper::init_sdl())
        {
            throw std::invalid_argument("Failed to init sdl");
        }

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

        if (!OpenGLWrapper::load_opengl_functions() ||
            !OpenGLWrapper::is_opengl_version_supported())
        {
            SDL_GL_DeleteContext(context_);
            SDL_DestroyWindow(window_);
            SDL_Quit();
            throw std::invalid_argument("Failed to load opengl");
        }
    }

    void render_impl()
    {
        SDL_GL_SwapWindow(window_);
    }

    void destroy_impl()
    {
        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

private:
    SDL_Window *window_    = nullptr;
    SDL_GLContext context_ = nullptr;
};

} // namespace SDL

template <> Engine::Instance<SDL::Engine> *Engine::create_instance()
{
    return new SDL::Engine();
}