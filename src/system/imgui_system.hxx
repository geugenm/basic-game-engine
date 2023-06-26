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

    void imgui_sprite_transform()
    {
        const std::string title =
            "Transform: " + m_sprite._texture._image_path.filename().string();
        ImGui::Begin(title.data());

        ImGui::Checkbox("Modify position", &m_dragging_sprite);

        if (m_dragging_sprite)
        {
        }

        ImGui::InputFloat("X position", &m_sprite._transform._position.x);
        ImGui::InputFloat("Y position", &m_sprite._transform._position.y);
        ImGui::InputFloat("Angle",
                          &m_sprite._transform._current_rotation_angle);

        ImGui::InputFloat("X scale", &m_sprite._transform._scale.x);
        ImGui::InputFloat("Y scale", &m_sprite._transform._scale.y);

        if (ImGui::Button("Apply"))
        {
            nlohmann::json output = m_sprite.serialize();
            const std::filesystem::path output_path =
                "../assets/sprites/" + m_sprite._name + ".json";
            std::ofstream output_file;
            output_file.open(output_path);

            if (output_file.is_open() == false)
            {
                throw std::invalid_argument(
                    "Could not write to file `" + output_path.string() +
                    ".json`. Current searching path: " +
                    std::filesystem::current_path().string());
            }

            std::cout << "Saved to `" + output_path.string() << std::endl;

            const std::string result = output.dump();
            output_file << result;
            output_file.close();
        }

        ImGui::End();
    }

    void handle_events()
    {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            m_dragging_sprite = false;
        }

        if (m_dragging_sprite == false)
        {
            return;
        }

        // Normalize mouse position
        m_mouse_position.x =
            ImGui::GetMousePos().x / ImGui::GetIO().DisplaySize.x;
        m_mouse_position.y =
            ImGui::GetMousePos().y / ImGui::GetIO().DisplaySize.y;

        // Convert to NDC
        m_sprite._transform._position.x = m_mouse_position.x * 2.0f - 1.0f;
        m_sprite._transform._position.y = 1.0f - m_mouse_position.y * 2.0f;

        // Handle mouse wheel rotation for scaling
        auto const &wheel_rotation = ImGui::GetIO().MouseWheel;
        if (wheel_rotation != 0.0f)
        {
            const float scale_factor =
                1.0f + wheel_rotation *
                           0.05f; // Adjust 0.1f to control the scaling speed
            m_sprite._transform._scale.x *= scale_factor;
            m_sprite._transform._scale.y *= scale_factor;
        }
    }

private:
    bool m_dragging_sprite = false;
    ImVec2 m_mouse_position;
    sprite &m_sprite;
};

class settings_window final
{
public:
    void update(entt::registry &registry, sdl_render_context *sdl_window)
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
    void show_window(entt::registry &registry, sdl_render_context *sdl_window)
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

    void apply_resolution_settings(sdl_render_context *sdl_window)
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
    void init(entt::registry &registry, entt::entity &entity)
    {
        auto &sdl_context = registry.get<sdl_render_context>(entity);

        imgui_subsdk::init_imgui(sdl_context._window, sdl_context._context);
    }

    void update(entt::registry &registry)
    {
        imgui_subsdk::new_frame();

        for (auto entity : registry.view<imgui_sprite_editor>())
        {
            auto &sprite_editor = registry.get<imgui_sprite_editor>(entity);
            sprite_editor.imgui_sprite_transform();
        }

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
                // create_new_game_popup();
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

    void handle_events(const SDL_Event &event, entt::registry &registry)
    {
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (m_tapped_game_creation == true)
            {
                m_tapped_game_creation = false;
            }
            else
            {
                m_tapped_mouse_position = ImGui::GetMousePos();
                m_tapped_game_creation  = true;
            }
        }

        if (event.type == SDL_EVENT_MOUSE_WHEEL)
        {
            ImGuiIO &io = ImGui::GetIO();
            if (event.wheel.y > 0)      // Upward scroll
            {
                io.MouseWheel += 1.0f;  // Increase mouse wheel value
            }
            else if (event.wheel.y < 0) // Downward scroll
            {
                io.MouseWheel -= 1.0f;  // Decrease mouse wheel value
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
    void create_new_game_popup()
    {
        if (m_tapped_game_creation == false)
        {
            return;
        }

        if (m_is_creating_new_game == true)
        {
            return;
        }
        // TODO: make adequate formula for size calculation
        ImGui::SetNextWindowPos(m_tapped_mouse_position);
        ImGui::SetNextWindowSize(ImVec2(160.0f, 40.0f));

        ImGui::Begin("##create_new_game", nullptr,
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoScrollbar |
                         ImGuiWindowFlags_NoTitleBar);

        imgui_subsdk::center_next_element_horizontally(140.0f);
        imgui_subsdk::center_next_element_vertically(25.0f);

        if (ImGui::Button("Create new game", ImVec2(140.0f, 25)))
        {
            m_is_creating_new_game = true;
        }

        if (m_is_creating_new_game)
        {
            create_game_window();
        }

        ImGui::End();
    }

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
                m_settings_window.make_visible();
            }

            ImGui::Separator();

            // Exit button
            if (ImGui::Button("Main menu", ImVec2(120, 0)))
            {
                current_state = game_states::in_menu;

                ImGui::CloseCurrentPopup();
                // Do something
            }

            // Resume button
            ImGui::SameLine();
            if (ImGui::Button("Resume", ImVec2(120, 0)))
            {
                current_state = game_states::played;
                ImGui::CloseCurrentPopup();
                // Do something
            }

            for (auto entity : registry.view<sdl_render_context>())
            {
                auto &window = registry.get<sdl_render_context>(entity);
                m_settings_window.update(registry, &window);
            }

            ImGui::EndPopup();
        }
    }

    void on_menu(entt::registry &registry, game_states &current_state)
    {
        for (auto entity : registry.view<sdl_render_context>())
        {
            auto &window = registry.get<sdl_render_context>(entity);
            m_main_menu_window.update(registry, &window);
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

    void create_slider(const char *name, int *value,
                       const uint32_t &width = 200.0f)
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

    [[nodiscard]] bool create_ok_button()
    {
        imgui_subsdk::center_next_element_horizontally(70.0f);

        if (ImGui::Button("OK", ImVec2(70.0f, 25)))
        {
            return true;
        }
        return false;
    }

    void create_game_window()
    {
        static bool stage_1 = false;
        static bool stage_2 = false;
        static bool stage_3 = false;

        // TODO: Get window size
        ImGui::SetNextWindowSize(ImVec2(1720, 1080));
        ImGui::SetNextWindowPos(ImVec2(100, 0));

        if (ImGui::Begin("Game concept", &m_is_creating_new_game,
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoScrollbar))
        {

            ImGui::PushItemWidth(200); // NOTE: (Push/Pop)ItemWidth is optional
            static char str2[128] = "Game #1";
            ImGui::InputText("##", str2, IM_ARRAYSIZE(str2));
            ImGui::PopItemWidth();

            ImGui::SameLine();

            ImGui::Text("Cost:");

            ImGui::Separator();

            ImGui::BeginChild(7, ImVec2(125, 124), true);

            ImGui::Text("Pick topic");

            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild(10, ImVec2(125, 124), true);

            ImGui::Text("Pick genre");

            ImGui::EndChild();

            ImGui::NewLine();

            ImGui::BeginChild(13, ImVec2(125, 124), true);

            ImGui::Text("Pick platform");

            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild(16, ImVec2(125, 124), true);

            ImGui::Text("Pick engine");

            ImGui::EndChild();

            const ImVec2 button_size(120, 30);
            if (ImGui::Button("Next", button_size))
            {
                m_is_creating_new_game = false;
                stage_1                = true;
            }
        }
        ImGui::End();

        static constexpr auto dev_stage_window_bounds = ImVec2(400, 400);
        // TODO: Make it a variable
        static constexpr auto context_window_bounds = ImVec2(1920, 1080);

        if (stage_1)
        {
            imgui_subsdk::center_on_screen(dev_stage_window_bounds,
                                           context_window_bounds);

            ImGui::SetNextWindowSize(dev_stage_window_bounds);

            if (ImGui::Begin("dev_stage_1", nullptr,
                             ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_NoScrollbar))
            {

                static int engine   = 0;
                static int gameplay = 0;
                static int story    = 0;

                create_slider("Engine", &engine, 200.0f);
                create_slider("Gameplay", &gameplay, 200.0f);
                create_slider("Story/Quests", &story, 200.0f);

                create_time_allocation_bar("1");

                if (create_ok_button())
                {
                    stage_1 = false;
                    stage_2 = true;
                }
            }
            ImGui::End();
        }

        if (stage_2)
        {

            imgui_subsdk::center_on_screen(dev_stage_window_bounds,
                                           context_window_bounds);

            ImGui::SetNextWindowSize(dev_stage_window_bounds);

            ImGui::NewLine();

            if (ImGui::Begin("dev_stage_2", nullptr,
                             ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_NoScrollbar))
            {

                static int dialogues    = 0;
                static int level_design = 0;
                static int ai           = 0;

                create_slider("Dialogues", &dialogues, 200.0f);
                create_slider("Level Design", &level_design, 200.0f);
                create_slider("AI", &ai, 200.0f);

                create_time_allocation_bar("2");

                if (create_ok_button())
                {
                    stage_2 = false;
                    stage_3 = true;
                }
            }
            ImGui::End();
        }

        if (stage_3)
        {
            imgui_subsdk::center_on_screen(dev_stage_window_bounds,
                                           context_window_bounds);

            ImGui::SetNextWindowSize(dev_stage_window_bounds);

            ImGui::NewLine();

            if (ImGui::Begin("dev_stage_3", nullptr,
                             ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_NoScrollbar))
            {
                static int world_design = 0;
                static int graphics     = 0;
                static int sound        = 0;

                create_slider("World Design", &world_design, 200.0f);
                create_slider("Graphics", &graphics, 200.0f);
                create_slider("Sound", &sound, 200.0f);

                create_time_allocation_bar("3");

                if (create_ok_button())
                {
                    stage_3 = false;
                }
            }
            ImGui::End();
        }
    }

    bool m_tapped_game_creation = false;

    bool m_is_creating_new_game = false;

    ImVec2 m_tapped_mouse_position;

    settings_window m_settings_window;

    main_menu_window m_main_menu_window;
};

} // namespace sdk
