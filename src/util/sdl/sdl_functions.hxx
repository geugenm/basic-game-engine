#pragma once

#include <SDL3/SDL.h>
#include <iostream>
#include <stdexcept>

#ifdef __ANDROID__

#include <GLES3/gl3.h>
#include <SDL3/SDL_main.h>

#else

#include <glad/glad.h>

#endif

#endif // __ANDROID__

namespace sdl_subsdk
{
void init_sdl();

SDL_Window *get_new_sdl_window(const char *window_title,
                               const int &window_width,
                               const int &window_height);

SDL_GLContext get_new_sdl_gl_context(SDL_Window *window);

void load_opengl_functions();

void init_opengl();
} // namespace sdl_subsdk
