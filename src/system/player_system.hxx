#pragma once

#include <entt/entt.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/vector_angle.hpp>

#include <SDL_events.h>
#include <general_components.hxx>

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
};

std::string get_vector2_info(const glm::vec2 &vector);

class player_system
{
public:
    void init(entt::registry &registry);

    void handle_events(const SDL_Event &event, entt::registry &registry);

    void update(entt::registry &registry,
                const sdl_render_context &render_window);

    [[nodiscard]] player &get_player()
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
        auto const &turret_sprite = registry.get<sprite>(m_player.m_turret);

        static const auto scale_matrix =
            glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.6f, 1.0f));
        const auto offset_matrix = glm::translate(
            scale_matrix,
            glm::vec3(turret_sprite._transform._position.x,
                      turret_sprite._transform._position.y, 0.0f));

        const auto transform = offset_matrix * aspect_matrix;

        glUseProgram(turret_sprite._shader._program_id);
        glUniformMatrix4fv(
            turret_sprite._shader.get_uniform_location("transform"), 1,
            GL_FALSE, glm::value_ptr(transform));
        glUseProgram(0);
    }

    player m_player;

    glm::vec2 m_window_size;
};

} // namespace sdk
