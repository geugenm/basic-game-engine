#pragma once

#include <imgui.h>
#include "tahoma.h"

namespace ImStyle
{

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
        const auto color   = color_index;
        ImVec4 &color_value = style.Colors[color_index];

        const bool is_background_dim_color = (color == ImGuiCol_ModalWindowDimBg ||
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

} // namespace ImStyle