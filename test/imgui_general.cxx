#include <SDL2/SDL.h>
#include <imgui.h>
#include <iostream>

#include <glad/glad.h>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <stdexcept>

// Function prototypes
void init_sdl();
void init_opengl(SDL_Window *window);
void init_imgui(SDL_Window *window, SDL_GLContext gl_context);
void cleanup(SDL_Window *window, SDL_GLContext gl_context);

int main(int argc, char *argv[]) {
    try {
        init_sdl();

        SDL_Window *window = SDL_CreateWindow("OpenGL 3.0 SDL ImGui Test",
                                              SDL_WINDOWPOS_UNDEFINED,
                                              SDL_WINDOWPOS_UNDEFINED,
                                              1280, 720,
                                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if (!window) {
            throw std::runtime_error("Failed to create window");
        }


        SDL_GLContext gl_context = SDL_GL_CreateContext(window);
        if (!gl_context) {
            throw std::runtime_error("Failed to create OpenGL 3.0 context");
        }

        init_opengl(window);
        init_imgui(window, gl_context);

        // Main loop
        bool running = true;
        SDL_Event event;

        while (running) {
            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(window);
            ImGui::NewFrame();

            // ImGui test window
            ImGui::ShowDemoWindow();

            ImGui::Render();
            glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(window);
        }

        cleanup(window, gl_context);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

void init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        throw std::runtime_error("Failed to initialize SDL");
    }
}

void init_opengl(SDL_Window *window) {
    if (!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void init_imgui(SDL_Window *window, SDL_GLContext gl_context) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void cleanup(SDL_Window *window, SDL_GLContext gl_context) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
