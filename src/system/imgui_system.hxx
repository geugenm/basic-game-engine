#pragma once

#include "imgui_wrapper.hxx"
#include "sdl_render_engine.hxx"

#include <entt/entt.hpp>

namespace sdk
{

struct imgui_window
{
    void update();
};

struct imgui_system
{
    explicit imgui_system(const sdl_render_context &sdl_render_context)
    {
        imgui_subsdk::init_imgui(sdl_render_context._window,
                                 sdl_render_context._context);
    }

    ~imgui_system() = default;

    void update(entt::registry &registry)
    {
        imgui_subsdk::new_frame();

        {
            // Add UI elements
            ImGui::SetNextWindowSize(ImVec2(400, 200));
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::Begin("Game UI", NULL,
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoCollapse);

            ImGui::ProgressBar(0.7f, ImVec2(-1, 0), "HP");
            if (ImGui::Button("Pause", ImVec2(80, 40)))
            {
                ImGui::OpenPopup("My Popup");
            }

            if (ImGui::BeginPopupModal("Pause", NULL,
                                       ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Pause menu");
                ImGui::Separator();

                if (ImGui::Button("Settings", ImVec2(120, 0)))
                {
                    // Do something
                }

                ImGui::Separator();

                // Exit button
                if (ImGui::Button("Exit", ImVec2(120, 0)))
                {
                    ImGui::CloseCurrentPopup();
                    // Do something
                }

                // Resume button
                ImGui::SameLine();
                if (ImGui::Button("Resume", ImVec2(120, 0)))
                {
                    ImGui::CloseCurrentPopup();
                    // Do something
                }

                ImGui::EndPopup();
            }

            ImGui::SameLine(ImGui::GetWindowWidth() - 80);
            ImGui::Text("<WeaponID>");

            ImGui::End();
        }

        // ImGui::ShowDemoWindow();

        // TODO: implement a normal event system
        //        auto view = registry.view<sdk::event>();
        //        imgui_subsdk::new_frame();
        //
        //        for (auto event : view) {
        //            auto const &sdk_event = view.get<sdk::event>(event);
        //            imgui_subsdk::process_event(sdk_event);
        //        }

        imgui_subsdk::render();
    }
};

} // namespace sdk
