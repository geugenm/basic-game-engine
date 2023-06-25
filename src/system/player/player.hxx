#pragma once

#include "entt/entity/fwd.hpp"
#include <entt/entt.hpp>
#include <game_logic.hxx>
#include <general_components.hxx>

namespace sdk
{

class player_system
{
public:
    void init(entt::registry &registry)
    {
        m_typing_hands_animation = registry.create();

        sprite typing_hands_animations =
            sprite::get_sprite_from_file("hands_typing");

        typing_hands_animations._transform = {
            ._position = glm::vec3(-0.244f, 0.115f, 0.3f),
            ._scale    = glm::vec3(0.044f, 0.044f, 1.0f)};

        registry.emplace<sprite>(m_typing_hands_animation,
                                 typing_hands_animations);

        registry.emplace<sprite_animation>(
            m_typing_hands_animation,
            sprite_animation::create_new_animation(2, 40));
    }

private:
    entt::entity m_typing_hands_animation{};
};

} // namespace sdk
