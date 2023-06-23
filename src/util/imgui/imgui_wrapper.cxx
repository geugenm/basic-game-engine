#include "imgui_wrapper.hxx"

#include "tahoma.h"

namespace imgui_subsdk
{

void init_imgui(SDL_Window *window, SDL_GLContext gl_context)
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

    setup_style(k_use_dark_style, k_window_alpha);

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

void setup_style(const bool &is_dark, const float &alpha)
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

    style.ChildBorderSize  = 1.0f;
    style.FrameBorderSize  = 0.0f;
    style.PopupBorderSize  = 1.0f;
    style.WindowBorderSize = 0.0f;
    style.WindowRounding   = 6.0f;
    style.FrameRounding    = 3.0f;

    style.Alpha         = 1.0f;
    style.ChildRounding = 6;
    style.GrabRounding  = 2;
    style.PopupRounding = 2;
    style.ScrollbarSize = 9;

    style.FramePadding = ImVec2(6, 3);
    style.ItemSpacing  = ImVec2(4, 4);

    style.WindowPadding     = ImVec2(15, 15);
    style.WindowRounding    = 5.0f;
    style.FramePadding      = ImVec2(5, 5);
    style.FrameRounding     = 4.0f;
    style.ItemSpacing       = ImVec2(12, 8);
    style.ItemInnerSpacing  = ImVec2(8, 6);
    style.IndentSpacing     = 25.0f;
    style.ScrollbarSize     = 15.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize       = 5.0f;
    style.GrabRounding      = 3.0f;

    for (int color_index = 0; color_index < ImGuiCol_COUNT; color_index++)
    {
        const auto color    = color_index;
        ImVec4 &color_value = style.Colors[color_index];

        const bool is_background_dim_color =
            (color == ImGuiCol_ModalWindowDimBg ||
             color == ImGuiCol_NavWindowingDimBg);
        const bool is_transparent_or_background =
            (color_value.w < 1.00f || color == ImGuiCol_FrameBg ||
             color == ImGuiCol_WindowBg || color == ImGuiCol_ChildBg);

        if (!is_background_dim_color && is_transparent_or_background)
        {
            color_value.w = alpha * color_value.w;
        }
    }

    ImGuiIO *io = &ImGui::GetIO();
    ImFontConfig font_cfg;
    font_cfg.FontDataOwnedByAtlas = false;
    io->Fonts->AddFontFromMemoryTTF((void *)tahoma, sizeof(tahoma), 17.f,
                                    &font_cfg);
}

void center_next_element_horizontally(const float &item_width)
{
    const float window_width = ImGui::GetWindowSize().x;

    const float item_x = (window_width - item_width) * 0.5f;
    ImGui::SetCursorPosX(item_x);
}

void center_next_element_vertically(const float &item_height)
{
    const float window_width = ImGui::GetWindowSize().y;

    const float item_y = (window_width - item_height) * 0.5f;
    ImGui::SetCursorPosY(item_y);
}

void center_on_screen(const float &item_width, const float &item_height,
                      const float &screen_width, const float &screen_height)
{
    const float item_x = (screen_width - item_width) * 0.5f;
    ImGui::SetCursorPosX(item_x);

    const float item_y = (screen_height - item_height) * 0.5f;
    ImGui::SetCursorPosY(item_y);

    ImGui::SetNextWindowPos(ImVec2(item_x, item_y));
}

void center_on_screen(const ImVec2 &item_bounds, const ImVec2 &window_bounds)
{
    center_on_screen(item_bounds.x, item_bounds.y, window_bounds.x,
                     window_bounds.y);
}
} // namespace imgui_subsdk