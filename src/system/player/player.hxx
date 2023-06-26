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

            typing_hands_animations._transform = {
                ._position = glm::vec3(-0.244f, 0.115f, 0.3f),
                ._scale    = glm::vec3(0.044f, 0.044f, 1.0f)};

            registry.emplace<sprite>(m_typing_hands_animation,
                                     typing_hands_animations);

            registry.emplace<sprite_animation>(
                m_typing_hands_animation,
                sprite_animation::create_new_animation(2, 40));
        }

        {
            m_typing_body_animation = registry.create();

            sprite typing_body = sprite::get_sprite_from_file("body_typing");

            typing_body._transform = {
                ._position = glm::vec3(-0.197f, 0.106f, 0.4f),
                ._scale    = glm::vec3(0.068f, 0.068f, 1.0f)};

            registry.emplace<sprite>(m_typing_body_animation, typing_body);

            registry.emplace<sprite_animation>(
                m_typing_body_animation,
                sprite_animation::create_new_animation(3, 24));
        }

        {
            m_computer_screen = registry.create();

            sprite compute_screen =
                sprite::get_sprite_from_file("computer_screen");

            compute_screen._transform = {
                ._position = glm::vec3(-0.181f, 0.141f, 0.2f),
                ._scale    = glm::vec3(0.030f, 0.050f, 1.0f)};

            registry.emplace<sprite>(m_computer_screen, compute_screen);

            registry.emplace<sprite_animation>(
                m_computer_screen,
                sprite_animation::create_new_animation(1, 46));
        }

        {
            m_head = registry.create();

            sprite head = sprite::get_sprite_from_file("head");

            head._transform = {
                ._position = glm::vec3(-0.193f, 0.104f, 0.7f),
                ._scale    = glm::vec3(0.163f, 0.163f, 1.0f),
            };

            registry.emplace<sprite>(m_head, head);
        }

        {
            m_pants = registry.create();

            sprite current_sprite = sprite::get_sprite_from_file("pants");

            current_sprite._transform = {
                ._position = glm::vec3(-0.206f, 0.092f, 0.2f),
                ._scale    = glm::vec3(0.163f, 0.163f, 1.0f),
            };

            registry.emplace<sprite>(m_pants, current_sprite);
        }

        {
            m_chair = registry.create();

            sprite current_sprite = sprite::get_sprite_from_file("chair");

            current_sprite._transform = {
                ._position               = glm::vec3(-0.206f, 0.092f, 0.2f),
                ._scale                  = glm::vec3(0.163f, 0.163f, 1.0f),
                ._current_rotation_angle = 0.090f};

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
