#include <SDL3/SDL.h>
#include <imgui.h>
#include <iostream>

#include <glad/glad.h>
#include <gtest/gtest.h>

#include "imgui_backends/imgui_impl_opengl3.h"
#include "imgui_backends/imgui_impl_sdl.h"
#include "imgui_engine.hxx"

#include <stdexcept>

void init_sdl();
void init_opengl(SDL_Window* window);
void init_imgui(SDL_Window* window, SDL_GLContext gl_context);
void cleanup(SDL_Window* window, SDL_GLContext gl_context);

TEST(ImGuiGeneralTest, BasicTest)
{
    try
    {
        init_sdl();

        SDL_Window* window =
            SDL_CreateWindow("OpenGL 3.0 SDL ImGui Test", 1280, 720, SDL_WINDOW_OPENGL);
        if (!window)
        {
            throw std::runtime_error("Failed to create window");
        }

        SDL_GLContext gl_context = SDL_GL_CreateContext(window);
        if (!gl_context)
        {
            throw std::runtime_error("Failed to create OpenGL 3.0 context");
        }

        init_opengl(window);
        init_imgui(window, gl_context);

        bool running = true;
        SDL_Event event;

        while (running)
        {
            while (SDL_PollEvent(&event))
            {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if (event.type == SDL_EVENT_QUIT)
                {
                    running = false;
                }
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(window);
            ImGui::NewFrame();

            ImGui::ShowDemoWindow();

            ImGui::Render();
            glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(window);
        }

        cleanup(window, gl_context);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        FAIL();
    }
}

TEST(ImGuiGeneralTest, ImGuiEngineTest) {
    const char* window_title        = "TestSDLEngine";
    constexpr int window_height = 1000;
    constexpr int window_width  = 1000;
    ImUI::Engine::Instance::instance().initialize(window_title, window_height, window_width);


    SDL_Event event;
    while (true)
    {
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
            {
                goto cleanup;
            }
        }

        ImUI::Engine::instance().render();
    }

cleanup:
    ImUI::Engine::Instance::instance().destroy();
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

void init_sdl()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        throw std::runtime_error("Failed to initialize SDL");
    }
}

void init_opengl(SDL_Window* window)
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

void init_imgui(SDL_Window* window, SDL_GLContext gl_context)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void cleanup(SDL_Window* window, SDL_GLContext gl_context)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}