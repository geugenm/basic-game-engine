#pragma once

#include "sdl_render_system.hxx"
#include <general_components.hxx>
#include <imgui.h>
#include <imgui_wrapper.hxx>

#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include <string_view>

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
        static constexpr auto points_window  = ImVec2(250, 200);
        static constexpr auto project_window = ImVec2(700, 200);

        // TODO: Make it a variable
        static constexpr auto context_window_bounds = ImVec2(1920, 1080);

        ImGui::SetNextWindowSize(points_window);

        const ImVec2 points_window_bounds(
            context_window_bounds.x - points_window.x, 0);
        ImGui::SetNextWindowPos(points_window_bounds);

        ImGui::Begin("Finance", nullptr,
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoScrollbar);

        static uint32_t time = 0;
        ImGui::Text("Date: %d", time);

        static uint32_t fans = 0;
        ImGui::Text("Fans: %d", fans);

        static uint32_t money = 0;
        ImGui::Text("Money: %d", money);

        ImGui::End();

        imgui_subsdk::center_on_screen(
            project_window, ImVec2(context_window_bounds.x, project_window.y));

        ImGui::SetNextWindowSize(project_window);

        ImGui::Begin("Research", nullptr,
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoScrollbar);

        {
            ImGui::BeginChild(28, ImVec2(125, 124), true);

            ImGui::Text("Bugs:");

            ImGui::EndChild();
        }

        ImGui::SameLine();

        {
            ImGui::BeginChild(29, ImVec2(125, 124), true);

            ImGui::Text("Art:");

            ImGui::EndChild();
        }

        ImGui::SameLine();

        {
            ImGui::BeginChild(30, ImVec2(125, 124), true);

            ImGui::Text("Project_name");

            ImGui::Text("Genre");

            ImGui::Text("Dev stage");

            ImGui::EndChild();
        }

        ImGui::SameLine();

        {
            ImGui::BeginChild(31, ImVec2(125, 124), true);

            ImGui::Text("Engine:");

            ImGui::EndChild();
        }

        ImGui::SameLine();

        {
            ImGui::BeginChild(32, ImVec2(125, 124), true);

            ImGui::Text("Study:");

            ImGui::EndChild();
        }

        ImGui::End();
    }

    void event_developement_stage()
    {
        static constexpr auto dev_stage_window_bounds = ImVec2(400, 400);
        // TODO: Make it a variable
        static constexpr auto context_window_bounds = ImVec2(1920, 1080);

        imgui_subsdk::center_on_screen(dev_stage_window_bounds,
                                       context_window_bounds);

        ImGui::SetNextWindowSize(dev_stage_window_bounds);

        if (ImGui::Begin("dev_stage_1", nullptr,
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoScrollbar))
        {

            create_slider("Engine", 200.0f);
            create_slider("Gameplay", 200.0f);
            create_slider("Story/Quests", 200.0f);

            create_time_allocation_bar("1");

            create_ok_button();
        }
        ImGui::End();

        imgui_subsdk::center_on_screen(dev_stage_window_bounds,
                                       context_window_bounds);

        ImGui::SetNextWindowSize(dev_stage_window_bounds);

        ImGui::NewLine();

        if (ImGui::Begin("dev_stage_2", nullptr,
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoScrollbar))
        {

            create_slider("Dialogues", 200.0f);
            create_slider("Level Design", 200.0f);
            create_slider("AI", 200.0f);

            create_time_allocation_bar("2");

            create_ok_button();
        }
        ImGui::End();

        imgui_subsdk::center_on_screen(dev_stage_window_bounds,
                                       context_window_bounds);

        ImGui::SetNextWindowSize(dev_stage_window_bounds);

        ImGui::NewLine();

        if (ImGui::Begin("dev_stage_3", nullptr,
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoScrollbar))
        {
            create_slider("World Design", 200.0f);
            create_slider("Graphic", 200.0f);
            create_slider("Sound", 200.0f);

            create_time_allocation_bar("3");

            create_ok_button();
        }
        ImGui::End();
    }

    void create_slider(const char *name, const uint32_t &width = 200.0f)
    {
        std::string id_sharps = "##";
        id_sharps.append(name);

        static int i27 = 42;
        ImGui::Text(name);
        imgui_subsdk::center_next_element_horizontally(200.0f);
        ImGui::PushItemWidth(200.0f);
        ImGui::DragInt(id_sharps.data(), &i27, 1, 0, 100, "%d%%",
                       ImGuiSliderFlags_AlwaysClamp);
        ImGui::PopItemWidth();
    }

    void create_time_allocation_bar(const std::string_view &id)
    {
        const float &window_width = ImGui::GetWindowSize().x;

        std::string final_id = "##";
        final_id.append(id);

        const ImGuiStyle &style = ImGui::GetStyle();
        const float offset      = 2.0f * style.WindowPadding.x;
        ImGui::BeginChild(110, ImVec2(window_width - offset, 100), true,
                          ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                              ImGuiWindowFlags_NoScrollbar);

        ImGui::Text("Time allocation (preview)");

        const float child_window_width = ImGui::GetWindowSize().x;

        ImGui::PushItemWidth(child_window_width - offset);
        static int i216 = 42;
        ImGui::DragInt(final_id.data(), &i216, 1, 0, 100, "%d%%",
                       ImGuiSliderFlags_AlwaysClamp);
        ImGui::PopItemWidth();

        ImGui::EndChild();
    }

    void create_ok_button()
    {
        imgui_subsdk::center_next_element_horizontally(70.0f);

        ImGui::Button("OK", ImVec2(70.0f, 25));
    }
};

} // namespace sdk
