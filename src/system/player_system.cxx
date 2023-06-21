#include "player_system.hxx"

#include <iostream>

namespace sdk
{
std::string get_vector2_info(const glm::vec2 &vector)
{
    std::stringstream ss;
    ss << "Vector position: [" << vector.x << "] [" << vector.y
       << "], Angle oriented(-pi to pi): "
       << glm::orientedAngle(glm::vec2(1.0f, 0.0f), glm::normalize(vector))
       << std::endl;
    return ss.str();
}

void player_system::init(entt::registry &registry)
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

void player_system::handle_events(const SDL_Event &event,
                                  entt::registry &registry)
{
    const Uint8 *keys = SDL_GetKeyboardState(nullptr);

    auto &hull_transform   = registry.get<sprite>(m_player.m_hull)._transform;
    auto &turret_transform = registry.get<sprite>(m_player.m_turret)._transform;

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
        hull_transform._current_rotation_angle += player::k_hull_rotation_speed;
    }

    if (keys[SDL_SCANCODE_D])
    {
        hull_transform._current_rotation_angle -= player::k_hull_rotation_speed;
    }

    turret_transform._position = hull_transform._position;

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        entt::entity bullet = registry.create();

        sprite bullet_sprite  = registry.get<sprite>(m_player.m_bullet_sample);
        bullet_sprite._shader = bullet_sprite._shader.get_initialized_shader();

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

    std::cout << "Mouse position: " << get_vector2_info(mouse_position);
    std::cout << "Turret position: "
              << get_vector2_info(turret_sprite._transform._position);
}

void player_system::update(entt::registry &registry,
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

    const glm::mat4 aspect_matrix =
        glm::scale(glm::mat4(1.0f), glm::vec3(window_aspect_ratio, 1.0f, 1.0f));

    handle_bullets(registry, aspect_matrix);
    handle_hull(registry, aspect_matrix);
    handle_turret(registry, aspect_matrix);
}
} // namespace sdk