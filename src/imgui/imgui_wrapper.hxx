#pragma once

#include <SDL3/SDL.h>
#include <imgui.h>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>

namespace ImWrapper
{

void setup_style(bool is_dark, float alpha)
{
    ImGuiStyle &style = ImGui::GetStyle();

    if (is_dark)
    {
        ImGui::StyleColorsDark();
    }
    else
    {
        ImGui::StyleColorsLight();
    }

    for (int i = 0; i <= ImGuiCol_COUNT; i++)
    {
        auto ei     = static_cast<ImGuiCol_>(i);
        ImVec4 &col = style.Colors[i];
        if ((ImGuiCol_ModalWindowDimBg != ei) &&
            (ImGuiCol_NavWindowingDimBg != ei) &&
            (col.w < 1.00f || (ImGuiCol_FrameBg == ei) ||
             (ImGuiCol_WindowBg == ei) || (ImGuiCol_ChildBg == ei)))
        {
            col.w = alpha * col.w;
        }
    }

    style.ChildBorderSize  = 1.0f;
    style.FrameBorderSize  = 0.0f;
    style.PopupBorderSize  = 1.0f;
    style.WindowBorderSize = 0.0f;
    style.WindowRounding   = 6.0f;
    style.FrameRounding    = 3.0f;
    style.Alpha            = 1.0f;
}

void init_imgui(SDL_Window *window, SDL_GLContext gl_context)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    setup_style(true, 0.8);

    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330 core");
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

} // namespace ImWrapper