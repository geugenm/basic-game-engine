#pragma once

#include "entt/entity/fwd.hpp"
#include <entt/entt.hpp>
#include <general_components.hxx>

namespace sdk
{

class player_system
{
public:
    void init(entt::registry &registry)
    {
        {
            m_typing_hands_animation = registry.create();

            sprite typing_hands_animations =
                sprite::get_sprite_from_file("hands_typing");

            registry.emplace<sprite>(m_typing_hands_animation,
                                     typing_hands_animations);

            registry.emplace<sprite_animation>(
                m_typing_hands_animation,
                sprite_animation::create_new_animation(2, 40));
        }

        {
            m_typing_body_animation = registry.create();

            sprite typing_body = sprite::get_sprite_from_file("body_typing");

            registry.emplace<sprite>(m_typing_body_animation, typing_body);

            registry.emplace<sprite_animation>(
                m_typing_body_animation,
                sprite_animation::create_new_animation(3, 24));
        }

        {
            m_computer_screen = registry.create();

            sprite compute_screen =
                sprite::get_sprite_from_file("computer_screen");

            registry.emplace<sprite>(m_computer_screen, compute_screen);

            registry.emplace<sprite_animation>(
                m_computer_screen,
                sprite_animation::create_new_animation(1, 46));
        }

        {
            m_head = registry.create();

            sprite head = sprite::get_sprite_from_file("head");

            registry.emplace<sprite>(m_head, head);
        }

        {
            m_pants = registry.create();

            sprite current_sprite = sprite::get_sprite_from_file("pants");

            registry.emplace<sprite>(m_pants, current_sprite);
        }

        {
            m_chair = registry.create();

            sprite current_sprite = sprite::get_sprite_from_file("chair");

            registry.emplace<sprite>(m_chair, current_sprite);
        }
    }

private:
    entt::entity m_typing_hands_animation{};
    entt::entity m_typing_body_animation{};
    entt::entity m_computer_screen{};
    entt::entity m_head{};
    entt::entity m_pants{};
    entt::entity m_chair{};
};

} // namespace sdk
