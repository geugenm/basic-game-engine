#pragma once

#include <SDL3/SDL.h>

#include "SDL_mouse.h"
#include "components/general_components.hxx"
#include "nlohmann/json_fwd.hpp"
#include "render/picopng.hxx"
#include "sdl_render_engine.hxx"

#include <entt/entt.hpp>
#include <opengl_functions.hxx>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <format>
#include <glm/gtx/vector_angle.hpp>
#include <nlohmann/json.hpp>

namespace sdk
{
struct opengl_texture_system final
{
    entt::entity _tank_hull;
    entt::entity _tank_turret;
    entt::entity _battlefield;

    void test(entt::registry &registry)
    {
        _tank_hull   = registry.create();
        _tank_turret = registry.create();

        sprite tank_hull   = sprite::get_sprite_from_file("hull");
        sprite tank_turret = sprite::get_sprite_from_file("turret");
        sprite battlefield = sprite::get_sprite_from_file("battlefield");

        // ! The order is important
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

    void handle_events(const SDL_Event &event)
    {
        if (event.type != SDL_EVENT_KEY_DOWN)
        {
            return;
        }

        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_W])
        {
            m_hull_parameters.position.x +=
                m_hull_parameters.moveSpeed *
                cos(m_hull_parameters.rotationAngle);
            m_hull_parameters.position.y +=
                m_hull_parameters.moveSpeed *
                sin(m_hull_parameters.rotationAngle);
            m_hull_parameters.position.x =
                std::clamp(m_hull_parameters.position.x,
                           -1.0f + m_hull_parameters.halfWidth,
                           1.0f - m_hull_parameters.halfWidth);
            m_hull_parameters.position.y =
                std::clamp(m_hull_parameters.position.y,
                           -1.0f + m_hull_parameters.halfHeight,
                           1.0f - m_hull_parameters.halfHeight);
        }

        if (keys[SDL_SCANCODE_S])
        {
            m_hull_parameters.position.x -=
                m_hull_parameters.moveSpeed *
                cos(m_hull_parameters.rotationAngle);
            m_hull_parameters.position.y -=
                m_hull_parameters.moveSpeed *
                sin(m_hull_parameters.rotationAngle);
            m_hull_parameters.position.x =
                std::clamp(m_hull_parameters.position.x,
                           -1.0f + m_hull_parameters.halfWidth,
                           1.0f - m_hull_parameters.halfWidth);
            m_hull_parameters.position.y =
                std::clamp(m_hull_parameters.position.y,
                           -1.0f + m_hull_parameters.halfHeight,
                           1.0f - m_hull_parameters.halfHeight);
        }

        if (keys[SDL_SCANCODE_A])
        {
            m_hull_parameters.rotationAngle += m_hull_parameters.rotateSpeed;
        }

        if (keys[SDL_SCANCODE_D])
        {
            m_hull_parameters.rotationAngle -= m_hull_parameters.rotateSpeed;
        }
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

        auto view_context = registry.view<sdl_render_context>();
        auto const &sdl_context =
            view_context.get<sdl_render_context>(window_entity);

        static const float aspect_ratio =
            static_cast<float>(sdl_context.get_width()) /
            static_cast<float>(sdl_context.get_height());
        const glm::mat4 aspect_matrix =
            glm::scale(glm::mat4(1.0f), glm::vec3(aspect_ratio, 1.0f, 1.0f));

        {
            auto transform = glm::mat4(1.0f);
            transform      = glm::scale(transform, glm::vec3(0.6f, 0.6f, 0.6f));
            transform      = glm::translate(
                transform, glm::vec3(m_hull_parameters.position.x,
                                          m_hull_parameters.position.y, 0.0f));
            transform = glm::rotate(transform, m_hull_parameters.rotationAngle,
                                    glm::vec3(0.0f, 0.0f, 1.0f));
            transform = transform * aspect_matrix;

            {
                auto const &tank_hull_sprite = view.get<sprite>(_tank_hull);
                glUseProgram(tank_hull_sprite._shader._program_id);

                glUniformMatrix4fv(
                    tank_hull_sprite._shader.get_uniform_location("transform"),
                    1, GL_FALSE, glm::value_ptr(transform));
                glUseProgram(0);
            }

            {
                static float mouse_x;
                static float mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);

                m_turret_parameters.position.x = m_hull_parameters.position.x;
                m_turret_parameters.position.y = m_hull_parameters.position.y;

                auto transform1 = glm::mat4(1.0f);
                transform1 =
                    glm::scale(transform1, glm::vec3(0.6f, 0.6f, 0.6f));
                transform1 = glm::translate(
                    transform1,
                    glm::vec3(m_turret_parameters.position.x,
                              m_turret_parameters.position.y, 0.0f));
                transform1 =
                    glm::rotate(transform1, m_turret_parameters.rotationAngle,
                                glm::vec3(0.0f, 0.0f, 1.0f));
                transform1                     = transform1 * aspect_matrix;
                auto const &tank_turret_sprite = view.get<sprite>(_tank_turret);
                glUseProgram(tank_turret_sprite._shader._program_id);
                glUniformMatrix4fv(
                    tank_turret_sprite._shader.get_uniform_location(
                        "transform"),
                    1, GL_FALSE, glm::value_ptr(transform1));
                glUseProgram(0);
            }
        }
        {
            auto transform = glm::mat4(1.0f);
            transform      = transform * aspect_matrix;

            auto const &battlefield_sprite = view.get<sprite>(_battlefield);
            glUseProgram(battlefield_sprite._shader._program_id);

            glUniformMatrix4fv(
                battlefield_sprite._shader.get_uniform_location("transform"), 1,
                GL_FALSE, glm::value_ptr(transform));
            glUseProgram(0);
        }
    }

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

    texture_params m_hull_parameters;
    texture_params m_turret_parameters;
};

} // namespace sdk
