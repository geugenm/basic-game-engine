#include "sdl_functions.hxx"

#include <glad/glad.h>
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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    constexpr Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

    SDL_Window *window = SDL_CreateWindow(window_title, window_width, window_height, flags);
    if (!window)
    {
        std::cerr << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to init SDL window with OpenGL.");
    }

    return window;
}

SDL_GLContext get_new_sdl_gl_context(SDL_Window *window)
{
    if (window == nullptr)
    {
        throw std::invalid_argument(
            "Can't create context from uninitialized SDL window.");
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        std::cerr << SDL_GetError() << std::endl;
        throw std::invalid_argument("Failed to create OpenGL context.");
    }

    return context;
}

void load_opengl_functions()
{
    if (!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to load OpenGL functions with glad: "
                  << glGetError() << std::endl;
        throw std::runtime_error("Can't load opengl functions.");
    }
}
void init_opengl()
{
    load_opengl_functions();

    if (!opengl_subsdk::is_opengl_version_supported())
    {
        throw std::runtime_error("OpenGL version is not supported.");
    }
}
} // namespace sdl_subsdk