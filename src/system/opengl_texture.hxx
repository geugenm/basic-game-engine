#pragma once

#include <SDL3/SDL.h>

#include "SDL_events.h"
#include "SDL_mouse.h"
#include "components/general_components.hxx"
#include "nlohmann/json_fwd.hpp"
#include "render/picopng.hxx"
#include "sdl_render_engine.hxx"

#include <entt/entity/entity.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/fwd.hpp>
#include <opengl_functions.hxx>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <format>
#include <glm/gtx/vector_angle.hpp>
#include <nlohmann/json.hpp>
#include <stack>

namespace sdk
{
struct opengl_texture_system final
{
    entt::entity _tank_hull{};
    entt::entity _tank_turret{};
    entt::entity _battlefield{};

    std::list<entt::entity> _bullets;
    entt::entity _bullet;

    void test(entt::registry &registry)
    {
        _tank_hull   = registry.create();
        _tank_turret = registry.create();
        _bullet      = registry.create();
        _battlefield = registry.create();

        sprite tank_hull   = sprite::get_sprite_from_file("hull");
        sprite tank_turret = sprite::get_sprite_from_file("turret");
        sprite battlefield = sprite::get_sprite_from_file("battlefield");

        sprite bullet = sprite::get_sprite_from_file("shell");

        // ! The order is important
        registry.emplace<sprite>(_bullet, bullet);

        registry.emplace<sprite>(_tank_turret, tank_turret);
        registry.emplace<sprite>(_tank_hull, tank_hull);
        registry.emplace<sprite>(_battlefield, battlefield);
    }

    void init_on(entt::registry &registry, entt::entity &window_entity) const
    {
        auto view_context = registry.view<sdl_render_context>();
        auto const &sdl_context =
            view_context.get<sdl_render_context>(window_entity);

        auto view = registry.view<sprite>();

        for (auto entity : view)
        {
            auto &entity_sprite = view.get<sprite>(entity);
            glUseProgram(entity_sprite._shader._program_id);
            initialize(entity_sprite._texture, sdl_context);
            glUseProgram(0);
        }

        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        {
            const auto &bullet_sprite = view.get<sprite>(_bullet);

            glUseProgram(bullet_sprite._shader._program_id);

            glUniform1i(bullet_sprite._shader.get_uniform_location("texture"),
                        bullet_sprite._texture._number);
            glUseProgram(0);
        }

        {
            const auto &tank_hull_sprite = view.get<sprite>(_tank_hull);
            glUseProgram(tank_hull_sprite._shader._program_id);

            glUniform1i(
                tank_hull_sprite._shader.get_uniform_location("texture"),
                tank_hull_sprite._texture._number);
            glUseProgram(0);
        }

        {
            const auto &tank_turret_sprite = view.get<sprite>(_tank_turret);

            glUseProgram(tank_turret_sprite._shader._program_id);

            glUniform1i(
                tank_turret_sprite._shader.get_uniform_location("texture"),
                tank_turret_sprite._texture._number);
            glUseProgram(0);
        }

        {
            const auto &battlefield_sprite = view.get<sprite>(_battlefield);

            glUseProgram(battlefield_sprite._shader._program_id);

            glUniform1i(
                battlefield_sprite._shader.get_uniform_location("texture"),
                battlefield_sprite._texture._number);
            glUseProgram(0);
        }
    }

    void handle_events(entt::registry &registry, const SDL_Event &event)
    {
        // TODO: implement frame time in order to fix blazing speed ups
        const Uint8 *keys = SDL_GetKeyboardState(nullptr);

        static constexpr float hull_rotation_speed = 0.02f;
        static constexpr float hull_movement_speed = 0.01f;

        glm::vec2 m_velocity(
            glm::cos(m_hull_transform._current_rotation_angle),
            glm::sin(m_hull_transform._current_rotation_angle));
        m_velocity *= hull_movement_speed;

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            auto view           = registry.view<sprite>();
            entt::entity bullet = registry.create();

            auto &bullet_go      = view.get<sprite>(_bullet);
            bullet_go._transform = m_turret_transform;

            sprite bullet_sprite  = view.get<sprite>(_bullet);
            bullet_sprite._shader = opengl_shader::get_new_shader(
                bullet_sprite._shader._vertex_source_path,
                bullet_sprite._shader._fragment_source_path);

            glUseProgram(bullet_sprite._shader._program_id);

            glUniform1i(bullet_sprite._shader.get_uniform_location("texture"),
                        bullet_sprite._texture._number);
            glUseProgram(0);

            bullet_sprite._transform = m_turret_transform;

            _bullets.push_back(bullet);

            registry.emplace<sprite>(bullet, bullet_sprite);
        }

        if (keys[SDL_SCANCODE_W])
        {
            m_hull_transform._position += m_velocity;
        }

        if (keys[SDL_SCANCODE_S])
        {
            m_hull_transform._position -= m_velocity;
        }

        if (keys[SDL_SCANCODE_A])
        {
            m_hull_transform._current_rotation_angle += hull_rotation_speed;
        }

        if (keys[SDL_SCANCODE_D])
        {
            m_hull_transform._current_rotation_angle -= hull_rotation_speed;
        }

        m_turret_transform._position = m_hull_transform._position;
    }

    void update(entt::registry &registry, entt::entity const &window_entity)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        auto view = registry.view<sprite>();

        for (auto entity : view)
        {
            auto const &entity_sprite = view.get<sprite>(entity);
            glUseProgram(entity_sprite._shader._program_id);
            auto target = static_cast<GLenum>(GL_TEXTURE0 +
                                              entity_sprite._texture._number);
            glActiveTexture(target);
            render(entity_sprite._texture);
            glUseProgram(0);
        }

        const auto view_context = registry.view<sdl_render_context>();
        auto const &sdl_context =
            view_context.get<sdl_render_context>(window_entity);

        static const float aspect_ratio =
            static_cast<float>(sdl_context.get_width()) /
            static_cast<float>(sdl_context.get_height());
        const glm::mat4 aspect_matrix =
            glm::scale(glm::mat4(1.0f), glm::vec3(aspect_ratio, 1.0f, 1.0f));

        const auto singular_matrix4 = glm::mat4(1.0f);
        const auto scale_matrix4 =
            glm::scale(singular_matrix4, glm::vec3(0.6f, 0.6f, 1.0f));
        const auto offset_matrix4 = glm::translate(
            scale_matrix4, glm::vec3(m_hull_transform._position.x,
                                     m_hull_transform._position.y, 0.0f));

        {
            for (auto const &bullet : _bullets)
            {
                auto &bullet_sprite = view.get<sprite>(bullet);

                const auto rotor = glm::vec2(
                    glm::cos(bullet_sprite._transform._current_rotation_angle),
                    glm::sin(bullet_sprite._transform._current_rotation_angle));

                static constexpr float velocity = 0.1f;
                bullet_sprite._transform._position += rotor * velocity;

                const auto scale_matrix =
                    glm::scale(singular_matrix4, glm::vec3(0.6f, 0.6f, 1.0f));
                const auto offset_matrix = glm::translate(
                    scale_matrix,
                    glm::vec3(bullet_sprite._transform._position.x,
                              bullet_sprite._transform._position.y, 0.0f));

                auto transform = glm::rotate(
                    offset_matrix,
                    bullet_sprite._transform._current_rotation_angle,
                    glm::vec3(0.0f, 0.0f, 1.0f));
                transform = transform * aspect_matrix;

                glUseProgram(bullet_sprite._shader._program_id);

                glUniformMatrix4fv(
                    bullet_sprite._shader.get_uniform_location("transform"), 1,
                    GL_FALSE, glm::value_ptr(transform));
                glUseProgram(0);
            }
        }

        {
            auto transform = glm::rotate(
                offset_matrix4, m_hull_transform._current_rotation_angle,
                glm::vec3(0.0f, 0.0f, 1.0f));
            transform                    = transform * aspect_matrix;
            auto const &tank_hull_sprite = view.get<sprite>(_tank_hull);
            glUseProgram(tank_hull_sprite._shader._program_id);

            glUniformMatrix4fv(
                tank_hull_sprite._shader.get_uniform_location("transform"), 1,
                GL_FALSE, glm::value_ptr(transform));
            glUseProgram(0);
        }

        {
            // TODO: fix the pi angle uneeded rotation for the whole 2pi (edge
            // case)
            // TODO: fix window proprotions position calculation bug
            auto &tank_turret_sprite = view.get<sprite>(_tank_turret);

            // Transforming SDL mouse coordinates to opengl texture coordinates
            glm::vec2 mouse_position;
            SDL_GetMouseState(&mouse_position.x, &mouse_position.y);

            const glm::vec2 window_size(sdl_context.get_width(),
                                        sdl_context.get_height());
            // Moving to the center of the window
            mouse_position = mouse_position - window_size / 2.0f;

            // Y-axis inversion (SDL coordinates)
            mouse_position.y *= -1.0f;
            mouse_position = glm::normalize(mouse_position);

            // Normalized X-axis vector for relative angle calculation
            auto axes_x_direction = glm::vec2(1.0f, 0.0f);

            float angle = glm::orientedAngle(axes_x_direction, mouse_position);

            // Linear interpolation for the angle in order to get smooth
            // rotation
            tank_turret_sprite._transform._current_rotation_angle =
                glm::mix(tank_turret_sprite._transform._current_rotation_angle,
                         angle, 0.007f);
            m_turret_transform = tank_turret_sprite._transform;

            auto transform1 = glm::rotate(
                offset_matrix4,
                tank_turret_sprite._transform._current_rotation_angle,
                glm::vec3(0.0f, 0.0f, 1.0f));
            transform1 = transform1 * aspect_matrix;

            glUseProgram(tank_turret_sprite._shader._program_id);
            glUniformMatrix4fv(
                tank_turret_sprite._shader.get_uniform_location("transform"), 1,
                GL_FALSE, glm::value_ptr(transform1));
            glUseProgram(0);
        }

        {
            //            auto transform = glm::mat4(1.0f);
            //            transform      = transform * aspect_matrix;
            //
            //            auto const &battlefield_sprite =
            //            view.get<sprite>(_battlefield);
            //            glUseProgram(battlefield_sprite._shader._program_id);
            //
            //            glUniformMatrix4fv(
            //                battlefield_sprite._shader.get_uniform_location("transform"),
            //                1, GL_FALSE, glm::value_ptr(transform));
            //            glUseProgram(0);
        }
    } // namespace sdk

private:
    static void render(opengl_texture const &texture)
    {
        glBindTexture(GL_TEXTURE_2D, texture._texture);

        glBindVertexArray(texture._VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
    }

    static void destroy(opengl_texture const &texture)
    {
        glDeleteVertexArrays(1, &texture._VAO);

        glDeleteBuffers(1, &texture._VBO);

        glDeleteBuffers(1, &texture._EBO);
    }

    static void initialize(opengl_texture &texture,
                           const sdl_render_context &context)
    {
        // Load image, create texture and generate mipmaps
        const auto png_data = get_png_data(texture);

        if (texture._width == 0 || texture._height == 0)
        {
            throw std::invalid_argument(
                "Given image bound are not determined. and Use get_png_data() "
                "before glTexImage2D.");
        }

        generate_buffers(texture);
        bind_buffers(texture, context);

        enable_attributes();
        glBindVertexArray(0);

        // Load and create a texture
        glGenTextures(1, &texture._texture);

        // All upcoming GL_TEXTURE_2D operations now have an effect on this
        // texture object
        glBindTexture(GL_TEXTURE_2D, texture._texture);

        set_texture_parameters(texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture._width, texture._height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, png_data.data());

        if (texture._need_generate_mipmaps)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        LOG(INFO) << "Texture loaded: " << texture._image_path.string();
    }

    [[nodiscard]] static std::vector<unsigned char>
    get_png_data(opengl_texture &texture)
    {
        if (!exists(texture._image_path))
        {
            throw std::invalid_argument("Failed to open PNG image: " +
                                        texture._image_path.string());
        }

        std::ifstream file(texture._image_path,
                           std::ios::binary | std::ios::ate);

        if (!file.is_open())
        {
            throw std::invalid_argument("Failed to open PNG image");
        }

        const std::streamsize file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<unsigned char> buffer(static_cast<size_t>(file_size));
        if (!file.read(reinterpret_cast<char *>(buffer.data()), file_size))
        {
            throw std::invalid_argument("Failed to read PNG image");
        }

        std::vector<unsigned char> png_data;
        png_data.reserve(static_cast<size_t>(file_size));

        if (decodePNG(png_data, texture._width, texture._height, buffer.data(),
                      static_cast<size_t>(file_size), true) != 0)
        {
            throw std::invalid_argument("Failed to decode PNG image");
        }

        if (png_data.empty())
        {
            throw std::invalid_argument("Empty PNG result");
        }

        return png_data;
    }

    static void generate_buffers(opengl_texture &texture)
    {
        glGenVertexArrays(1, &texture._VAO);

        glGenBuffers(1, &texture._VBO);

        glGenBuffers(1, &texture._EBO);
    }

    static void bind_buffers(opengl_texture &texture,
                             const sdl_render_context &sdl_context)
    {
        if (texture._needs_to_be_scaled)
        {
            float image_aspect_ratio = texture.get_image_aspect_ratio();
            float window_aspect_ratio =
                static_cast<float>(sdl_context.get_width()) /
                static_cast<float>(sdl_context.get_height());

            float scale_x;
            float scale_y;

            if (image_aspect_ratio > window_aspect_ratio)
            {
                scale_x = 1.0f;
                scale_y = window_aspect_ratio / image_aspect_ratio;
            }
            else
            {
                scale_x = image_aspect_ratio / window_aspect_ratio;
                scale_y = 1.0f;
            }

            // Apply scale to the vertices
            for (size_t i = 0; i < texture._vertices.size() / 3; ++i)
            {
                texture._vertices[i * 3] *= scale_x;
                texture._vertices[i * 3 + 1] *= scale_y;
            }

            // Apply scale to the texture coordinates
            for (size_t i = 0; i < texture._vertices.size() / 3; ++i)
            {
                texture._vertices[i * 3 + 2] *= scale_x;
                texture._vertices[i * 3 + 3] *= scale_y;
            }
        }

        glBindVertexArray(texture._VAO);

        glBindBuffer(GL_ARRAY_BUFFER, texture._VBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(texture._vertices.size() * sizeof(GLfloat)),
            texture._vertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texture._EBO);

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(texture._indices.size() * sizeof(GLuint)),
            texture._indices.data(), GL_DYNAMIC_DRAW);
    }

    static void enable_attributes()
    {
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              nullptr);

        glEnableVertexAttribArray(0);

        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              (GLvoid *)(3 * sizeof(GLfloat)));

        glEnableVertexAttribArray(1);

        // TexCoord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              (GLvoid *)(6 * sizeof(GLfloat)));

        glEnableVertexAttribArray(2);
    }

    static void set_texture_parameters(opengl_texture const &texture)
    {
        // Set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set texture filtering parameters
        if (texture._need_generate_mipmaps)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    transform m_hull_transform;

    transform m_turret_transform;
};

} // namespace sdk
