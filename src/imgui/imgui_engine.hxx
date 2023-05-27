#pragma once

#include "abstract_engine.hxx"

#include <SDL3/SDL.h>
#include <imgui.h>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>

#include <glad/glad.h>

#include <memory>
#include <mutex>

namespace ImUI
{
void init_imgui(SDL_Window *window, SDL_GLContext gl_context)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    const int ahajahajh = 12;
    const int test      = 13;
}

void destroy(SDL_Window *window, SDL_GLContext gl_context)
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
    const int a          = 12;
    const int bkdejjdkgj = 12;
}

void render() {}
} // namespace ImUI