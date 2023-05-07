#pragma once

#include "opengl_functions.hxx"
#include "sdl_shader.hxx"
#include <abstract_engine.hxx>

#include <glad/glad.h>

namespace SDLEngine
{

class Instance : public Engine::Instance<Instance>
{
public:
    template <typename... Args> void initialize_impl(Args&&... args)
    {
        if (!GL::init_sdl())
        {
            return ;
        }

        window_ = GL::create_window("Test", 1000, 1000);
        if (!window_)
        {
            SDL_Quit();
            return ;
        }

        context_ = GL::create_opengl_context(window_);
        if (!context_)
        {
            SDL_DestroyWindow(window_);
            SDL_Quit();
            return ;
        }

        if (!GL::load_opengl_functions() || !GL::is_opengl_version_supported())
        {
            SDL_GL_DeleteContext(context_);
            SDL_DestroyWindow(window_);
            SDL_Quit();
            return ;
        }

        compile_shaders();
    }


    template <typename... Args>
    void render_impl(Args&&... args)
    {
        SDL_GL_SwapWindow(window_);
        GL::listen_opengl_errors();
    }

    template <typename... Args> void destroy_impl(Args&&... args)
    {
        glDeleteProgram(program_id_);
        GL::listen_opengl_errors();

        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

private:
    SDL_Window* window_    = nullptr;
    SDL_GLContext context_ = nullptr;
    GLuint program_id_     = 0;

    OpenGLShader * shaders_;

    void compile_shaders();
};

} // namespace Engine
