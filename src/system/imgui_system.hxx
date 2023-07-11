#pragma once

#include "sdl_render_system.hxx"
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_mouse.h>
#include <SDL_video.h>
#include <filesystem>
#include <general_components.hxx>
#include <imgui.h>
#include <imgui_wrapper.hxx>

#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include <string_view>

namespace sdk
{

class imgui_sprite_editor final
{
public:
    explicit imgui_sprite_editor(sprite &sprite) : m_sprite(sprite) {}

    void render()
    {
        const std::string title =
            "Transform: " + m_sprite._texture._image_path.filename().string();
        ImGui::BeginChild(title.data());

        ImGui::Checkbox("Modify position", &m_dragging_sprite);

        ImGui::InputFloat("X position", &m_sprite._transform._position.x);
        ImGui::InputFloat("Y position", &m_sprite._transform._position.y);
        ImGui::InputFloat("Z position", &m_sprite._transform._position.z);
        ImGui::InputFloat("Angle",
                          &m_sprite._transform._current_rotation_angle);

        ImGui::InputFloat("X scale", &m_sprite._transform._scale.x);
        ImGui::InputFloat("Y scale", &m_sprite._transform._scale.y);
        ImGui::InputFloat("Z scale", &m_sprite._transform._scale.z);

        ImGui::InputText("Sprite name", m_sprite._name.data(), 4096);

        apply();

        ImGui::EndChild();
    }

    void handle_events()
    {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            m_dragging_sprite = false;
        }

        if (!m_dragging_sprite)
        {
            return;
        }

        // Get mouse position in pixels
        m_mouse_position.x = ImGui::GetMousePos().x;
        m_mouse_position.y = ImGui::GetMousePos().y;

        // Convert to NDC for transformations
        float ndc_x =
            m_mouse_position.x / ImGui::GetIO().DisplaySize.x * 2.0f - 1.0f;
        float ndc_y =
            1.0f - m_mouse_position.y / ImGui::GetIO().DisplaySize.y * 2.0f;

        // Update sprite's position
        m_sprite._transform._position.x = ndc_x;
        m_sprite._transform._position.y = ndc_y;

        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        {
            // Handle mouse wheel rotation for scaling
            auto const &wheel_rotation = ImGui::GetIO().MouseWheel;
            if (wheel_rotation != 0.0f)
            {
                m_sprite._transform._position.z += wheel_rotation * 0.05f;
            }
            return;
        }

        // Handle mouse wheel rotation for scaling
        auto const &wheel_rotation = ImGui::GetIO().MouseWheel;
        if (wheel_rotation != 0.0f)
        {
            const float scale_factor = 1.0f + wheel_rotation * 0.05f;
            m_sprite._transform._scale.x *= scale_factor;
            m_sprite._transform._scale.y *= scale_factor;
        }
    }

    [[nodiscard]] const sprite &get_sprite() const
    {
        return m_sprite;
    }

private:
    void apply() const
    {
        if (!ImGui::Button("Apply"))
        {
            return;
        }

        m_sprite.save_to_file();
    }

    bool m_dragging_sprite = false;
    ImVec2 m_mouse_position;
    sprite &m_sprite;
};

class settings_window final
{
public:
    void update(entt::registry const &registry, sdl_render_context *sdl_window)
    {
        if (sdl_window == nullptr)
        {
            throw std::invalid_argument(
                "Failed to open resolution settings: `sdl_window` is null");
        }

        if (m_is_window_opened)
        {
            show_window(registry, sdl_window);
        }

        if (m_is_showing_debug_window)
        {
            ImGui::ShowDebugLogWindow();
        }
    }

    void make_visible()
    {
        m_is_window_opened = true;
    }

private:
    void show_window([[maybe_unused]] entt::registry const &registry,
                     sdl_render_context *sdl_window)
    {
        if (ImGui::Begin("Settings", &m_is_window_opened))
        {
            ImGui::Text("FPS:");

            ImGui::PushItemWidth(200);
            ImGui::DragInt("##fps", &m_chosen_fps, 1, 25, 700);
            ImGui::PopItemWidth();

            ImGui::Checkbox("VSync", &m_use_vsync);

            ImGui::SameLine();

            ImGui::Checkbox("Show Debug", &m_is_showing_debug_window);

            ImGui::Text("Resolution:");

            ImGui::PushItemWidth(200);
            ImGui::ListBox("##resolution_list", &m_chosen_resolution,
                           m_resolution_items.data(), m_resolution_items.size(),
                           5);
            ImGui::PopItemWidth();

            if (ImGui::Button("Apply"))
            {
                apply_resolution_settings(sdl_window);
            }
        }
        ImGui::End();
    }

    void apply_resolution_settings(sdl_render_context *sdl_window) const
    {
        if (m_chosen_resolution == 0)
        {
            SDL_SetWindowSize(sdl_window->_window, 7680, 4320);
        }
        if (m_chosen_resolution == 1)
        {
            SDL_SetWindowSize(sdl_window->_window, 3840, 2160);
        }
        if (m_chosen_resolution == 2)
        {
            SDL_SetWindowSize(sdl_window->_window, 1920, 1080);
        }
        if (m_chosen_resolution == 3)
        {
            SDL_SetWindowSize(sdl_window->_window, 720, 480);
        }
        if (m_chosen_resolution == 4)
        {
            SDL_SetWindowSize(sdl_window->_window, 640, 480);
        }
    }

    bool m_is_window_opened = false;

    bool m_is_showing_debug_window = false;

    bool m_use_vsync = false;

    int m_chosen_resolution = 2;

    int m_chosen_fps = 60;

    static constexpr std::array<const char *, 5> m_resolution_items = {
        "8K", "4K", "FullHD", "HD", "480p"};
};

class main_menu_window final
{
public:
    void update(entt::registry &registry, sdl_render_context *sdl_window)
    {
        show_window(registry, sdl_window);
    }

private:
    void show_window(entt::registry &registry, sdl_render_context *sdl_window)
    {
        ImGui::OpenPopup("Menu");

        if (ImGui::BeginPopupModal("Menu", nullptr,
                                   ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (ImGui::Button("Play", ImVec2(120, 0)))
            {
                for (auto entity : registry.view<game_states>())
                {
                    auto &current_state = registry.get<game_states>(entity);
                    current_state       = game_states::played;
                }
            }

            if (ImGui::Button("Settings", ImVec2(120, 0)))
            {
                m_settings_window.make_visible();
            }

            if (ImGui::Button("Exit", ImVec2(120, 0)))
            {
                for (auto entity : registry.view<game_states>())
                {
                    auto &current_state = registry.get<game_states>(entity);
                    current_state       = game_states::exited;
                }
                ImGui::CloseCurrentPopup();
            }

            m_settings_window.update(registry, sdl_window);

            ImGui::EndPopup();
        }
    }

    settings_window m_settings_window;
};

struct imgui_system
{
    void init(entt::registry &registry, entt::entity const &sdl_window_entity)
    {
        auto &sdl_context = registry.get<sdl_render_context>(sdl_window_entity);

        imgui_subsdk::init_imgui(sdl_context._window, sdl_context._context);

        for (auto ent : registry.view<imgui_sprite_editor>())
        {
            auto const &sprite_editor = registry.get<imgui_sprite_editor>(ent);
            m_sprite_editors.push_back(sprite_editor.get_sprite()._name);
        }
    }

    void update(entt::registry &registry)
    {
        imgui_subsdk::new_frame();

        for (auto entity : registry.view<game_states>())
        {
            using sdk::game_states;

            auto &state = registry.get<game_states>(entity);

            if (state == game_states::in_menu)
            {
                on_menu(registry);
            }

            if (state == game_states::played)
            {
                show_sprite_editor(registry);
                create_pause_button(state);
            }

            if (state == game_states::paused)
            {
                on_pause(registry, state);
            }
        }

        imgui_subsdk::render();
    }

    void handle_events(const SDL_Event &event, entt::registry &registry)
    {
        if (event.type == SDL_EVENT_MOUSE_WHEEL)
        {
            ImGuiIO &io = ImGui::GetIO();
            if (event.wheel.y > 0)
            {
                io.MouseWheel += 1.0f;
            }
            else if (event.wheel.y < 0)
            {
                io.MouseWheel -= 1.0f;
            }
        }

        if (ImGui::IsKeyDown(ImGuiKey_Escape))
        {

            for (auto entity : registry.view<game_states>())
            {
                auto &state = registry.get<game_states>(entity);

                state = game_states::paused;
            }
        }

        for (auto entity : registry.view<imgui_sprite_editor>())
        {
            auto &sprite_editor = registry.get<imgui_sprite_editor>(entity);
            sprite_editor.handle_events();
        }
    }

private:
    void create_pause_button(game_states &state)
    {
        const ImVec2 displaySize = ImGui::GetIO().DisplaySize;

        ImGui::SetNextWindowPos(ImVec2(displaySize.x, 0), ImGuiCond_Once,
                                ImVec2(1, 0));

        ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav |
            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration;

        ImGui::Begin("Pause Button", nullptr, windowFlags);

        if (ImGui::Button("| |"))
        {
            state = game_states::paused;
        }

        ImGui::End();
    }

    void show_sprite_editor(entt::registry &registry)
    {
        static std::size_t selected_index = 0;

        if (ImGui::BeginCombo("List", m_sprite_editors[selected_index].data()))
        {
            for (size_t i = 0; i < m_sprite_editors.size(); ++i)
            {
                const std::string &item = m_sprite_editors[i];
                const bool isSelected   = (selected_index == i);
                if (ImGui::Selectable(item.c_str(), isSelected))
                {
                    selected_index = i;
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        for (auto entity : registry.view<imgui_sprite_editor>())
        {
            auto &sprite_editor = registry.get<imgui_sprite_editor>(entity);
            if (m_sprite_editors[selected_index] ==
                sprite_editor.get_sprite()._name)
            {
                sprite_editor.render();
            }
        }

        if (ImGui::Button("Save All"))
        {
            for (auto entity : registry.view<imgui_sprite_editor>())
            {
                auto const &sprite_editor =
                    registry.get<imgui_sprite_editor>(entity);
                sprite_editor.get_sprite().save_to_file();
            }
        }
    }

    void on_pause(entt::registry &registry, game_states &current_state)
    {
        ImGui::OpenPopup("Pause");

        if (ImGui::BeginPopupModal("Pause", nullptr,
                                   ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Pause menu");

            ImGui::Separator();
            if (ImGui::Button("Resume", ImVec2(120, 0)))
            {
                current_state = game_states::played;
                ImGui::CloseCurrentPopup();
            }

            ImGui::Separator();
            if (ImGui::Button("Settings", ImVec2(120, 0)))
            {
                m_settings_window.make_visible();
            }

            ImGui::Separator();

            if (ImGui::Button("Main menu", ImVec2(120, 0)))
            {
                current_state = game_states::in_menu;

                ImGui::CloseCurrentPopup();
            }

            for (auto entity : registry.view<sdl_render_context>())
            {
                auto &window = registry.get<sdl_render_context>(entity);
                m_settings_window.update(registry, &window);
            }

            ImGui::EndPopup();
        }
    }

    void on_menu(entt::registry &registry)
    {
        for (auto entity : registry.view<sdl_render_context>())
        {
            auto &window = registry.get<sdl_render_context>(entity);
            m_main_menu_window.update(registry, &window);
        }
    }

    settings_window m_settings_window;

    main_menu_window m_main_menu_window;

    std::vector<std::string> m_sprite_editors;
};

} // namespace sdk
