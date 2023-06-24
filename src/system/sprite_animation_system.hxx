#pragma once

#include "general_components.hxx"

#include <entt/entt.hpp>
#include <iostream>

namespace sdk
{

struct sprite_animation final
{
    std::size_t _current_frame{};
    std::size_t _rows{};
    std::size_t _columns{};

    [[nodiscard]] static sprite_animation
    create_new_animation(const std::size_t &rows, const std::size_t &cols)
    {
        if (rows == 0 || cols == 0)
        {
            throw std::invalid_argument(
                "Can't create sprite animation: `rows={}` or "
                "`cols={}` is 0");
        }

        sprite_animation new_animation{
            ._current_frame = 0,
            ._rows          = rows,
            ._columns       = cols,
        };

        return new_animation;
    }

    [[nodiscard]] bool is_properly_initialized() const
    {
        if (_rows == 0 || _columns == 0)
        {

            std::cout << "WARNING: sprite animation is not properly "
                         "initialized: `rows={}` or "
                         "`cols={}` is 0";
            return false;
        }

        return true;
    }
};

class sprite_animation_system
{
public:
    void update(entt::registry &registry)
    {
        for (auto entity : registry.view<sprite, sprite_animation>())
        {

            auto &current_animation = registry.get<sprite_animation>(entity);

            if (current_animation.is_properly_initialized() == false)
            {
                throw std::invalid_argument(
                    "Can't update frame: `animation` is not properly "
                    "initialized.");
            }

            auto &current_sprite = registry.get<sprite>(entity);

            next_frame(current_animation, current_sprite);
        }
    }

private:
    static void update_texture_coordinates(const sprite_animation &animation,
                                           sprite &sprite)
    {
        if (sprite._texture._vertices.empty())
        {
            throw std::invalid_argument(
                "Can't update frame: `vertices` is empty.");
        }

        if (animation._columns == 1 && animation._rows == 1)
        {
            std::cout << "WARNING: No frames, just a sprite, skipping update..."
                      << std::endl;
            return;
        }

        const int X = 0;
        const int Y = 1;

        const float frameWidth  = 1.f / static_cast<float>(animation._columns);
        const float frameHeight = 1.f / static_cast<float>(animation._rows);

        const auto floated_rows = static_cast<float>(
            animation._rows - animation._current_frame / animation._columns);
        const auto floated_columns =
            static_cast<float>(animation._current_frame % animation._columns);

        auto &vertices = sprite._texture._vertices;

        const auto first_vertex    = sprite._texture.get_tex_coord_index(0);
        vertices[first_vertex + X] = frameWidth * floated_columns;
        vertices[first_vertex + Y] = frameHeight * floated_rows;

        const auto second_vertex    = sprite._texture.get_tex_coord_index(1);
        vertices[second_vertex + X] = frameWidth * (floated_columns + 1);
        vertices[second_vertex + Y] = frameHeight * floated_rows;

        const auto third_vertex    = sprite._texture.get_tex_coord_index(2);
        vertices[third_vertex + X] = frameWidth * (floated_columns + 1);
        vertices[third_vertex + Y] = frameHeight * (floated_rows + 1);

        const auto fourth_vertex    = sprite._texture.get_tex_coord_index(3);
        vertices[fourth_vertex + X] = frameWidth * floated_columns;
        vertices[fourth_vertex + Y] = frameHeight * (floated_rows + 1);
    }

    static void next_frame(sprite_animation &animation, sprite &sprite)
    {
        const auto max_frame = animation._columns * animation._rows - 1;

        if (max_frame == animation._current_frame)
        {
            animation._current_frame = 0;
        }
        else
        {
            animation._current_frame++;
        }

        update_texture_coordinates(animation, sprite);
    }
};

} // namespace sdk
