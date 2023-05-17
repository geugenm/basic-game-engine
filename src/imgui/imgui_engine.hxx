#pragma once

#include "abstract_engine.hxx"

#include <SDL3/SDL.h>
#include <imgui.h>

#include <imgui_backends/imgui_impl_opengl3.h>
#include <imgui_backends/imgui_impl_sdl.h>

#include <glad/glad.h>

#include <memory>
#include <mutex>

namespace ImUI
{
class Engine : public ::Engine::Instance<Engine>
{
public:
    void initialize_impl(const char* window_title, const int & window_width, const int & window_height)
    {
        init_sdl();

        window_ =
            SDL_CreateWindow(window_title, window_width, window_height, SDL_WINDOW_OPENGL);
        if (!window_)
        {
            throw std::runtime_error("Failed to create window");
        }

        gl_context_ = SDL_GL_CreateContext(window_);
        if (!gl_context_)
        {
            throw std::runtime_error("Failed to create OpenGL 3.0 context");
        }

        init_opengl();
        init_imgui();
    }

    void render_impl()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window_);
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window_);
    }

    void destroy_impl() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(gl_context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

private:
    static void init_sdl()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
        {
            throw std::runtime_error("Failed to initialize SDL");
        }
    }

    static void init_opengl()
    {
        if (!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    }

    void init_imgui()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForOpenGL(window_, gl_context_);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    SDL_Window* window_;
    SDL_GLContext gl_context_;
};

} // namespace ImUI

template <>::Engine::Instance<ImUI::Engine>* ImUI::Engine::Instance::create_instance()
{
    return new ImUI::Engine();
}