#pragma once

#include "player_system.hxx"
#include <entt/entity/fwd.hpp>
#include <glm/common.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace sdk
{

class enemy_system
{
public:
    void init(entt::registry &registry)
    {
        m_control_system.init(registry);
    }

    void update(entt::registry &registry,
                const sdl_render_context &render_window, const player &player)
    {
        auto const &player_transform =
            registry.get<sprite>(player.m_hull)._transform;
        auto &enemy_transform =
            registry.get<sprite>(m_control_system.m_player.m_hull)._transform;
        auto &enemy_transform_turret =
            registry.get<sprite>(m_control_system.m_player.m_turret)._transform;

        m_control_system.update(registry, render_window);

        const auto direction = player_transform._position;
        
        m_control_system.m_turret_target_rotation_angle = glm::orientedAngle(
            glm::vec2(1.0f, 0.0f), player_transform._position);
    }

private:
    player_system m_control_system;

    static constexpr float k_detection_range = 10.0f;
};

} // namespace sdk
