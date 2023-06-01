#pragma once

#include "opengl_functions.hxx"
#include <abstract_engine.hxx>

#include <glad/glad.h>

namespace sdl_sdk
{

class engine : public sdk::engine
{
public:
    engine(const char *window_title, const int &height, const int width)
        : k_window_title_(window_title), k_window_height_(height),
          k_window_width_(width)
    {
    }

    ~engine() override = default;

    void initialize() override
    {
        OpenGLWrapper::init_sdl();

        window_ = OpenGLWrapper::get_new_sdl_window(
            k_window_title_, k_window_width_, k_window_height_);
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

    void render() override
    {
        SDL_GL_SwapWindow(window_);
    }

    void destroy() override
    {
        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

    [[nodiscard]] const SDL_Window * get_window() const {
        if (!is_initialized()) {
            throw sdk::engine_error("Trying to get the uninitialized window.", "get_window");
        }
        return window_;
    }

    [[nodiscard]] SDL_GLContext get_context() const {
        if (!is_initialized()) {
            throw sdk::engine_error("Trying to get the uninitialized context.", "get_context");
        }
        return context_;
    }

protected:
    [[nodiscard]] SDL_Window * access_window() {
        if (!is_initialized()) {
            throw sdk::engine_error("Trying to get the uninitialized window.", "get_window");
        }
        return window_;
    }

private:
    SDL_Window *window_    = nullptr;
    SDL_GLContext context_ = nullptr;

    const char *k_window_title_;
    const int k_window_height_;
    const int k_window_width_;
};

} // namespace sdl_sdk