#include <SDL3/SDL.h>
#include <imgui.h>
#include <iostream>

#include <glad/glad.h>
#include <gtest/gtest.h>

#include "imgui_wrapper.hxx"
#include "opengl_functions.hxx"

#include <stdexcept>

TEST(ImGuiGeneralTest, BasicTest)
{
    try
    {
        OpenGLWrapper::init_sdl();

        SDL_Window *window = OpenGLWrapper::get_new_sdl_window(
            "OpenGL 3.0 SDL ImGui Test", 1280, 720);

        SDL_GLContext gl_context =
            OpenGLWrapper::get_new_sdl_gl_context(window);

        OpenGLWrapper::init_opengl();
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