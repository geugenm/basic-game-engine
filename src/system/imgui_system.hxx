#pragma once

#include "components/general_components.hxx"
#include "imgui_wrapper.hxx"
#include "sdl_render_engine.hxx"

#include <entt/entt.hpp>

namespace sdk
{

struct imgui_system
{
    void init(entt::registry &registry, entt::entity &entity)
    {
        auto view         = registry.view<sdl_render_context>();
        auto &sdl_context = view.get<sdl_render_context>(entity);

        imgui_subsdk::init_imgui(sdl_context._window, sdl_context._context);
    }

    void update(entt::registry &registry)
    {
        imgui_subsdk::new_frame();

        // Add UI elements
        ImGui::SetNextWindowSize(ImVec2(400, 200));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Game UI", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoCollapse);

        ImGui::ProgressBar(0.7f, ImVec2(-1, 0), "HP");

        if (ImGui::Button("Pause", ImVec2(80, 40)))
        {
            for (auto entity : registry.view<game_states>())
            {
                auto &state =
                    registry.view<game_states>().get<game_states>(entity);
                state = game_states::paused;
            }
            ImGui::OpenPopup("Pause");
        }

        if (ImGui::BeginPopupModal("Pause", nullptr,
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
                for (auto entity : registry.view<game_states>())
                {
                    auto &state =
                        registry.view<game_states>().get<game_states>(entity);
                    state = game_states::exited;
                }
                ImGui::CloseCurrentPopup();
                // Do something
            }

            // Resume button
            ImGui::SameLine();
            if (ImGui::Button("Resume", ImVec2(120, 0)))
            {
                for (auto entity : registry.view<game_states>())
                {
                    auto &state =
                        registry.view<game_states>().get<game_states>(entity);
                    state = game_states::played;
                }
                ImGui::CloseCurrentPopup();
                // Do something
            }

            ImGui::EndPopup();
        }

        ImGui::SameLine(ImGui::GetWindowWidth() - 80);
        ImGui::Text("<WeaponID>");

        ImGui::End();

        ImGui::ShowDebugLogWindow();

        imgui_subsdk::render();
    }
};

} // namespace sdk
