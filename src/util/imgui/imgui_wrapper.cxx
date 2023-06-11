#include "imgui_wrapper.hxx"
#include "imgui_style.hxx"


void imgui_subsdk::init_imgui(SDL_Window *window, SDL_GLContext gl_context)
{
    if (window == nullptr)
    {
        throw std::invalid_argument("Given SDL _window is nullptr.");
    }

    if (gl_context == nullptr)
    {
        throw std::invalid_argument("Given SDL GL _context is nullptr.");
    }

    IMGUI_CHECKVERSION();
    if (ImGui::CreateContext() == nullptr)
    {
        throw std::invalid_argument("Failed to create ImGui _context.");
    }

    ImStyle::setup_style(k_use_dark_style, k_window_alpha);

    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(k_opengl_version.data());
}
void imgui_subsdk::destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}
void imgui_subsdk::new_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}
void imgui_subsdk::process_event(const SDL_Event &event)
{
    ImGui_ImplSDL3_ProcessEvent(&event);
}
void imgui_subsdk::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void imgui_subsdk::render(ImDrawData *draw_data)
{
    if (draw_data == nullptr)
    {
        throw std::invalid_argument("Given nullptr draw data.");
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
