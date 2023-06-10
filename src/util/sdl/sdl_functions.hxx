#pragma once

#include "event.hxx"

#include <SDL3/SDL.h>
#include <iostream>
#include <stdexcept>

#ifndef OPENGL_MAJOR_VERSION
#define OPENGL_MAJOR_VERSION 3
#endif

#ifndef OPENGL_MINOR_VERSION
#define OPENGL_MINOR_VERSION 2
#endif

namespace sdl_subsdk
{
void init_sdl();

SDL_Window *get_new_sdl_window(const char *window_title,
                               const int &window_width,
                               const int &window_height);

SDL_GLContext get_new_sdl_gl_context(SDL_Window *window);

void load_opengl_functions();

void init_opengl();

sdk::keyboard sdl_key_to_sdk_key(const int &keycode);
} // namespace sdl_subsdk
