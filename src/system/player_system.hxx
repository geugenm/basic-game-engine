#pragma once

#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/vector_angle.hpp>

#include "../components/general_components.hxx"
#include <SDL_events.h>

namespace sdk
{

struct player
{
    entt::entity m_turret;
    entt::entity m_hull;

    entt::entity m_bullet_sample;
    std::list<entt::entity> m_bullets;

    static constexpr float k_hull_rotation_speed = 0.02f;
    static constexpr float k_hull_movement_speed = 0.01f;

    static constexpr float k_turret_rotation_speed = 0.008f;
};

std::stringstream get_vector2_info(const glm::vec2 &vector)
{
    std::stringstream ss;
    ss << "Vector position: [" << vector.x << "] [" << vector.y
       << "], Angle oriented(-pi to pi): "
       << glm::orientedAngle(glm::vec2(1.0f, 0.0f), glm::normalize(vector))
       << std::endl;
    return ss;
}

class player_system
{
public:
    void init(entt::registry &registry)
    {
        m_player.m_turret = registry.create();
        m_player.m_hull   = registry.create();

        sprite tank_hull   = sprite::get_sprite_from_file("hull");
        sprite tank_turret = sprite::get_sprite_from_file("turret");

        sprite bullet = sprite::get_sprite_from_file("shell");

        registry.emplace<sprite>(m_player.m_turret, tank_turret);
        registry.emplace<sprite>(m_player.m_hull, tank_hull);

        m_player.m_bullet_sample = registry.create();
        registry.emplace<sprite>(m_player.m_bullet_sample, bullet);

        auto sprite_shader_view = registry.view<sprite>();

        // Init shaders via getting new compiled shader
        for (auto entity : sprite_shader_view)
        {
            auto &ent_sprite   = sprite_shader_view.get<sprite>(entity);
            ent_sprite._shader = ent_sprite._shader.get_initialized_shader();
        }

        // Attach textures to shaders
        for (auto entity : sprite_shader_view)
        {
            auto const &ent_sprite = sprite_shader_view.get<sprite>(entity);
            glUseProgram(ent_sprite._shader._program_id);

            glUniform1i(ent_sprite._shader.get_uniform_location("texture"),
                        ent_sprite._texture._number);
            glUseProgram(0);
        }
    }

    void handle_events(const SDL_Event &event, entt::registry &registry)
    {
        const Uint8 *keys = SDL_GetKeyboardState(nullptr);

        auto &hull_transform = registry.get<sprite>(m_player.m_hull)._transform;
        auto &turret_transform =
            registry.get<sprite>(m_player.m_turret)._transform;

        glm::vec2 m_velocity(glm::cos(hull_transform._current_rotation_angle),
                             glm::sin(hull_transform._current_rotation_angle));
        m_velocity *= player::k_hull_movement_speed;

        if (keys[SDL_SCANCODE_W])
        {
            hull_transform._position += m_velocity;
        }

        if (keys[SDL_SCANCODE_S])
        {
            hull_transform._position -= m_velocity;
        }

        if (keys[SDL_SCANCODE_A])
        {
            hull_transform._current_rotation_angle +=
                player::k_hull_rotation_speed;
        }

        if (keys[SDL_SCANCODE_D])
        {
            hull_transform._current_rotation_angle -=
                player::k_hull_rotation_speed;
        }

        turret_transform._position = hull_transform._position;

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            entt::entity bullet = registry.create();

            sprite bullet_sprite =
                registry.get<sprite>(m_player.m_bullet_sample);
            bullet_sprite._shader =
                bullet_sprite._shader.get_initialized_shader();

            glUseProgram(bullet_sprite._shader._program_id);

            glUniform1i(bullet_sprite._shader.get_uniform_location("texture"),
                        bullet_sprite._texture._number);
            glUseProgram(0);

            bullet_sprite._transform = turret_transform;

            m_player.m_bullets.push_back(bullet);

            registry.emplace<sprite>(bullet, bullet_sprite);
        }

        auto const &turret_sprite = registry.get<sprite>(m_player.m_turret);

        // Transforming SDL mouse coordinates to opengl texture coordinates
        glm::vec2 mouse_position;
        SDL_GetMouseState(&mouse_position.x, &mouse_position.y);

        // Moving to the center of the window
        mouse_position = mouse_position - m_window_size / 2.0f;

        // Y-axis inversion (SDL coordinates)
        mouse_position.y *= -1.0f;

        mouse_position = glm::normalize(mouse_position);

        LOG(INFO) << get_vector2_info(mouse_position).str();
        LOG(INFO) << get_vector2_info(turret_sprite._transform._position).str();
        LOG(INFO) << get_vector2_info(mouse_position -
                                      turret_sprite._transform._position)
                         .str();

        // Normalized X-axis vector for relative angle calculation
        auto axes_x_direction = glm::vec2(1.0f, 0.0f);

        m_turret_target_rotation_angle = glm::orientedAngle(
            glm::normalize(axes_x_direction), glm::normalize(mouse_position));
    }

    void update(entt::registry &registry,
                const sdl_render_context &render_window)
    {
        auto const &hull_sprite = registry.get<sprite>(m_player.m_hull);
        hull_sprite.render();

        auto const &turret_sprite = registry.get<sprite>(m_player.m_turret);
        turret_sprite.render();

        for (auto const &ent_bullet : m_player.m_bullets)
        {
            auto const &entity_sprite = registry.get<sprite>(ent_bullet);
            entity_sprite.render();
        }

        m_window_size =
            glm::vec2(render_window.get_width(), render_window.get_height());

        const float window_aspect_ratio =
            static_cast<float>(render_window.get_width()) /
            static_cast<float>(render_window.get_height());

        const glm::mat4 aspect_matrix = glm::scale(
            glm::mat4(1.0f), glm::vec3(window_aspect_ratio, 1.0f, 1.0f));

        handle_bullets(registry, aspect_matrix);
        handle_hull(registry, aspect_matrix);
        handle_turret(registry, aspect_matrix);
    }

    [[nodiscard]] player get_player() const
    {
        return m_player;
    }

private:
    void handle_bullets(entt::registry &registry,
                        const glm::mat4 &aspect_matrix)
    {
        static constexpr float velocity = 0.1f;
        std::list<entt::entity> entities_to_remove;
        static const auto scale_matrix =
            glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.6f, 1.0f));

        for (auto const &bullet : m_player.m_bullets)
        {
            auto &bullet_sprite = registry.get<sprite>(bullet);

            // Check if the bullet is inside the battlefield
            if (bullet_sprite._transform._position.x < -2.0f ||
                bullet_sprite._transform._position.x > 2.0f ||
                bullet_sprite._transform._position.y < -2.0f ||
                bullet_sprite._transform._position.y > 2.0f)
            {
                // Mark outside bullets for deletion
                entities_to_remove.push_back(bullet);
                continue;
            }

            const auto rotor = glm::vec2(
                glm::cos(bullet_sprite._transform._current_rotation_angle),
                glm::sin(bullet_sprite._transform._current_rotation_angle));

            bullet_sprite._transform._position += rotor * velocity;

            const auto offset_matrix = glm::translate(
                scale_matrix,
                glm::vec3(bullet_sprite._transform._position.x,
                          bullet_sprite._transform._position.y, 0.0f));

            auto transform = glm::rotate(
                offset_matrix, bullet_sprite._transform._current_rotation_angle,
                glm::vec3(0.0f, 0.0f, 1.0f));
            transform = transform * aspect_matrix;

            glUseProgram(bullet_sprite._shader._program_id);

            glUniformMatrix4fv(
                bullet_sprite._shader.get_uniform_location("transform"), 1,
                GL_FALSE, glm::value_ptr(transform));
            glUseProgram(0);
        }

        for (const auto &entity : entities_to_remove)
        {
            m_player.m_bullets.remove(entity);
            registry.destroy(entity);
        }
    }

    void handle_hull(entt::registry &registry, const glm::mat4 &aspect_matrix)
    {
        auto const &hull_sprite = registry.get<sprite>(m_player.m_hull);

        static const auto scale_matrix =
            glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.6f, 1.0f));
        const auto offset_matrix = glm::translate(
            scale_matrix, glm::vec3(hull_sprite._transform._position.x,
                                    hull_sprite._transform._position.y, 0.0f));

        const auto rotation = glm::rotate(
            offset_matrix, hull_sprite._transform._current_rotation_angle,
            glm::vec3(0.0f, 0.0f, 1.0f));
        const auto transform = rotation * aspect_matrix;

        auto const &tank_hull_sprite = registry.get<sprite>(m_player.m_hull);
        glUseProgram(tank_hull_sprite._shader._program_id);

        glUniformMatrix4fv(
            tank_hull_sprite._shader.get_uniform_location("transform"), 1,
            GL_FALSE, glm::value_ptr(transform));
        glUseProgram(0);
    }

    void handle_turret(entt::registry &registry, const glm::mat4 &aspect_matrix)
    {
        // TODO: fix the pi angle uneeded rotation for the whole 2pi (edge
        // case)
        // TODO: fix window proprotions position calculation bug
        auto &turret_sprite = registry.get<sprite>(m_player.m_turret);

        static const auto scale_matrix =
            glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.6f, 1.0f));
        const auto offset_matrix = glm::translate(
            scale_matrix,
            glm::vec3(turret_sprite._transform._position.x,
                      turret_sprite._transform._position.y, 0.0f));

        // Linear interpolation for the angle in order to get smooth
        // rotation
        turret_sprite._transform._current_rotation_angle =
            m_turret_target_rotation_angle;
        
        const auto rotor = glm::vec2(
            glm::cos(turret_sprite._transform._current_rotation_angle),
            glm::sin(turret_sprite._transform._current_rotation_angle));

        const auto rotation =
            glm::rotate(offset_matrix, m_turret_target_rotation_angle,
                        glm::vec3(0.0f, 0.0f, 1.0f));
        const auto transform = rotation * aspect_matrix;

        glUseProgram(turret_sprite._shader._program_id);
        glUniformMatrix4fv(
            turret_sprite._shader.get_uniform_location("transform"), 1,
            GL_FALSE, glm::value_ptr(transform));
        glUniform2f(turret_sprite._shader.get_uniform_location("direction"),
                    rotor.x, rotor.y);
        glUseProgram(0);
    }

    player m_player;

    glm::vec2 m_window_size;

    float m_turret_target_rotation_angle = 0.0f;

    friend class enemy_system;
};

} // namespace sdk
