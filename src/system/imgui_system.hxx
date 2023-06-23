#pragma once

#include "sdl_render_system.hxx"
#include <general_components.hxx>
#include <imgui.h>
#include <imgui_wrapper.hxx>

#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>

namespace sdk
{

struct imgui_system
{
    void init(entt::registry &registry, entt::entity &entity)
    {
        auto &sdl_context = registry.get<sdl_render_context>(entity);

        imgui_subsdk::init_imgui(sdl_context._window, sdl_context._context);
    }

    void update(entt::registry &registry)
    {
        imgui_subsdk::new_frame();

        for (auto entity : registry.view<game_states>())
        {
            auto &state = registry.get<game_states>(entity);

            if (state == game_states::in_menu)
            {
                on_menu(registry, state);
            }

            if (state == game_states::played)
            {
                on_play(registry, state);
            }

            if (state == game_states::paused)
            {
                on_pause(registry, state);
            }

            if (state == game_states::exited)
            {
                on_exit(registry, state);
            }
        }

        imgui_subsdk::render();
    }

private:
    void on_pause(entt::registry &registry, game_states &current_state)
    {
        ImGui::OpenPopup("Pause");

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
            if (ImGui::Button("Main menu", ImVec2(120, 0)))
            {
                for (auto entity : registry.view<game_states>())
                {
                    auto &state = registry.get<game_states>(entity);
                    state       = game_states::in_menu;
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
                    auto &state = registry.get<game_states>(entity);
                    state       = game_states::played;
                }
                ImGui::CloseCurrentPopup();
                // Do something
            }

            ImGui::EndPopup();
        }
    }

    void on_menu(entt::registry &registry, game_states &current_state)
    {
        ImGui::OpenPopup("Menu");

        if (ImGui::BeginPopupModal("Menu", nullptr,
                                   ImGuiWindowFlags_AlwaysAutoResize))
        {

            ImGui::Separator();
            if (ImGui::Button("Play", ImVec2(120, 0)))
            {
                current_state = game_states::played;
            }

            ImGui::SameLine();
            if (ImGui::Button("Campaign (Soon...)", ImVec2(120, 0)))
            {
                // TODO: ...
            }

            ImGui::Separator();

            if (ImGui::Button("Settings (Soon...)", ImVec2(120, 0)))
            {
                // TODO: ...
            }

            ImGui::SameLine();
            if (ImGui::Button("Exit", ImVec2(120, 0)))
            {
                current_state = game_states::exited;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void on_exit(entt::registry &registry, game_states &current_state) {}

    void on_play(entt::registry &registry, game_states &current_state)
    {
        ImGui::Begin("Finance");

        static int cash = 1000;
        ImGui::Text("Cash: %d", cash);
        if (ImGui::Button("Increase Cash")) {
            cash += 100;
        }

        ImGui::End();

        ImGui::Begin("Research");

        static int research_points = 0;
        ImGui::Text("Research Points: %d", research_points);
        if (ImGui::Button("Gain Research Point")) {
            research_points++;
        }

        ImGui::End();

        ImGui::Begin("Development");

        static char game_name[32] = "";
        ImGui::InputText("Game Name", game_name, IM_ARRAYSIZE(game_name));
        if (ImGui::Button("Develop Game")) {
            // Add code to develop the game here
        }

        ImGui::End();
    }
};

} // namespace sdk
