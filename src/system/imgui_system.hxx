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

        show_sprite_editor(registry);

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

    std::vector<std::string> m_sprite_editors;
};

} // namespace sdk
