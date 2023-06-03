#pragma once

#include "opengl_functions.hxx"
#include <abstract_engine.hxx>

#include <glad/glad.h>

#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

namespace sdl_subsdk
{

class engine : public sdk::engine
{
public:
    engine(const char *window_title, const int &height, const int width,
           const char *engine_name = "sdl_engine")
        : sdk::engine(engine_name), k_window_title_(window_title),
          k_window_height_(height), k_window_width_(width)
    {
        LOG(INFO) << "Engine created";
    }

    ~engine() override = default;

    [[nodiscard]] SDL_Window *get_window()
    {
        if (!is_initialized())
        {
            throw sdk::engine_error("Trying to get the uninitialized window.",
                                    "get_window");
        }
        return window_;
    }

    [[nodiscard]] SDL_GLContext get_context()
    {
        if (!is_initialized())
        {
            throw sdk::engine_error("Trying to get the uninitialized window.",
                                    "get_window");
        }
        return context_;
    }

protected:
    void initialize_impl() override
    {
        opengl_subsdk::init_sdl();

        window_ = opengl_subsdk::get_new_sdl_window(
            k_window_title_, k_window_width_, k_window_height_);
        if (!window_)
        {
            SDL_Quit();
            throw std::invalid_argument("Failed to create SDL window");
        }

        context_ = opengl_subsdk::get_new_sdl_gl_context(window_);
        if (!context_)
        {
            SDL_DestroyWindow(window_);
            SDL_Quit();
            throw std::invalid_argument("Failed to create OpenGL context");
        }

        opengl_subsdk::init_opengl();
    }

    void render_impl() override
    {
        SDL_GL_SwapWindow(window_);
    }

    void destroy_impl() override
    {
        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

private:
    SDL_Window *window_    = nullptr;
    SDL_GLContext context_ = nullptr;

    const char *k_window_title_;
    const int k_window_height_;
    const int k_window_width_;
};

} // namespace sdl_subsdk