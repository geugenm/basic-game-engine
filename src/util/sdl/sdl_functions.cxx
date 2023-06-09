#include "sdl_functions.hxx"

#include <opengl_functions.hxx>

namespace sdl_subsdk
{
void init_sdl()
{
    constexpr Uint32 flags = SDL_INIT_VIDEO;
    if (SDL_Init(flags) != 0)
    {
        throw std::runtime_error(
            "Failed to init SDL (SDL_INIT_VIDEO flag given)");
    }
}

SDL_Window *get_new_sdl_window(const char *window_title,
                               const int &window_width,
                               const int &window_height)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                        OPENGL_CURRENT_MAJOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                        OPENGL_CURRENT_MINOR_VERSION);

    constexpr Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

    SDL_Window *window =
        SDL_CreateWindow(window_title, window_width, window_height, flags);
    if (!window)
    {
        std::cerr << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to init SDL _window with OpenGL.");
    }

    return window;
}

SDL_GLContext get_new_sdl_gl_context(SDL_Window *window)
{
    if (window == nullptr)
    {
        throw std::invalid_argument(
            "Can't create _context from uninitialized SDL _window.");
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        std::cerr << SDL_GetError() << std::endl;
        throw std::invalid_argument("Failed to create OpenGL _context.");
    }

    return context;
}

void load_opengl_functions()
{
#ifndef __ANDROID__
    if (!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to load OpenGL functions with glad: "
                  << glGetError() << std::endl;
        throw std::runtime_error("Can't load opengl functions.");
    }
#endif
}
void init_opengl()
{
    load_opengl_functions();

    if (!opengl_subsdk::is_opengl_version_supported())
    {
        throw std::invalid_argument("OpenGL version is not supported.");
    }
}
} // namespace sdl_subsdk