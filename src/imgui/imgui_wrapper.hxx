#pragma once

#include <SDL3/SDL.h>
#include <imgui.h>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>

#include "imgui_style.hxx"

namespace imgui_subsdk
{
constexpr bool k_use_dark_style             = true;
constexpr float k_window_alpha              = 0.8f;
constexpr std::string_view k_opengl_version = "#version 150 core";

void init_imgui(SDL_Window *window, SDL_GLContext gl_context)
{
    if (window == nullptr)
    {
        throw sdk::engine_error("Given SDL window is nullptr.", "init_imgui");
    }

    if (gl_context == nullptr)
    {
        throw sdk::engine_error("Given SDL GL context is nullptr.",
                                "init_imgui");
    }

    IMGUI_CHECKVERSION();
    if (ImGui::CreateContext() == nullptr)
    {
        throw sdk::engine_error("Failed to create ImGui context.",
                                "init_imgui");
    }

    ImStyle::setup_style(k_use_dark_style, k_window_alpha);

    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(k_opengl_version.data());
}

void destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void new_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void process_event(const SDL_Event &event)
{
    ImGui_ImplSDL3_ProcessEvent(&event);
}

void render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void render(ImDrawData *draw_data)
{
    if (draw_data == nullptr)
    {
        throw sdk::engine_error("Given nullptr draw data.", "imgui::render");
    }

    ImGuiIO const &io = ImGui::GetIO();

    const auto frame_buffer_width =
        static_cast<size_t>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    const auto frame_buffer_height =
        static_cast<size_t>(io.DisplaySize.y * io.DisplayFramebufferScale.y);

    if (frame_buffer_width == 0 || frame_buffer_height == 0)
    {
        return;
    }

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace imgui_subsdk