#include <SDL3/SDL.h>
#include <imgui.h>
#include <iostream>

#include <glad/glad.h>
#include <gtest/gtest.h>

#include "imgui_wrapper.hxx"
#include "opengl_functions.hxx"

#include <stdexcept>

void init_sdl();
void init_opengl();

TEST(ImGuiGeneralTest, BasicTest)
{
    try
    {
        init_sdl();

        SDL_Window *window = SDL_CreateWindow("OpenGL 3.0 SDL ImGui Test", 1280,
                                              720, SDL_WINDOW_OPENGL);
        if (!window)
        {
            throw std::runtime_error("Failed to create window");
        }

        SDL_GLContext gl_context = SDL_GL_CreateContext(window);
        if (!gl_context)
        {
            throw std::runtime_error("Failed to create OpenGL 3.0 context");
        }

        init_opengl();
        OpenGLWrapper::enable_debug_mode();
        ImWrapper::init_imgui(window, gl_context);

        bool running = true;
        SDL_Event event;

        while (running)
        {
            while (SDL_PollEvent(&event))
            {
                ImWrapper::process_event(event);
                if (event.type == SDL_EVENT_QUIT)
                {
                    running = false;
                }
            }

            ImWrapper::new_frame();

            ImGui::ShowDemoWindow();

            glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImWrapper::render();

            SDL_GL_SwapWindow(window);
        }

        ImWrapper::destroy();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        FAIL();
    }
}

auto main(int argc, char **argv) -> int
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

void init_opengl()
{
    if (!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}
