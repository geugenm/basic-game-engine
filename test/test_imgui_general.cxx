#include <SDL3/SDL.h>
#include <imgui.h>
#include <iostream>

#include <glad/glad.h>
#include <gtest/gtest.h>

#include "imgui_wrapper.hxx"
#include "opengl_functions.hxx"
#include "sdl_engine.hxx"

#include <stdexcept>

TEST(ImGuiGeneralTest, BasicTest)
{
    try
    {
        sdl_subsdk::init_sdl();

        SDL_Window *window = sdl_subsdk::get_new_sdl_window(
            "OpenGL 3.0 SDL ImGui Test", 1280, 720);

        SDL_GLContext gl_context =
            sdl_subsdk::get_new_sdl_gl_context(window);

        sdl_subsdk::init_opengl();
        opengl_subsdk::enable_debug_mode();
        imgui_subsdk::init_imgui(window, gl_context);

        bool running = true;
        SDL_Event event;

        while (running)
        {
            while (SDL_PollEvent(&event))
            {
                imgui_subsdk::process_event(event);
                if (event.type == SDL_EVENT_QUIT)
                {
                    running = false;
                }
            }

            imgui_subsdk::new_frame();

            ImGui::ShowDemoWindow();

            glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);

            imgui_subsdk::render();

            SDL_GL_SwapWindow(window);
        }

        imgui_subsdk::destroy();
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