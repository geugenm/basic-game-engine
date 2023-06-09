#pragma once

#include <SDL3/SDL.h>
#include <imgui.h>

#include <stdexcept>
#include <string_view>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>

namespace imgui_subsdk
{
constexpr bool k_use_dark_style             = true;
constexpr float k_window_alpha              = 0.8f;
constexpr std::string_view k_opengl_version = "#version 300 es";

void setup_style(const bool &is_dark, const float &alpha);

void init_imgui(SDL_Window *window, SDL_GLContext gl_context);

void destroy();

void new_frame();

void process_event(const SDL_Event &event);

void render();

void render(ImDrawData *draw_data);

void center_next_element_horizontally(const float &item_width);

void center_next_element_vertically(const float &item_height);

void center_on_screen(const float &item_height, const float &item_width,
                      const float &screen_width, const float &screen_height);

void center_on_screen(const ImVec2 &item_bounds, const ImVec2 &window_bounds);
} // namespace imgui_subsdk