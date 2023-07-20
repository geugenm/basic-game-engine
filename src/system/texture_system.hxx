#pragma once

#include "imgui_system.hxx"
#include "opengl_texture.hxx"
#include "sdl_render_system.hxx"
#include "sprite_animation_system.hxx"
#include <file_loading_stuff.hxx>

#include <SDL_events.h>
#include <SDL_mouse.h>

#include <general_components.hxx>

#include <opengl_functions.hxx>

#include <entt/entt.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>

#include <stb_image.h>
#include <string>

namespace sdk
{

class opengl_texture_system final
{
public:
    void test(entt::registry &registry)
    {
        m_garage = registry.create();

        sprite battlefield = sprite::get_sprite_from_file("level1");

        registry.emplace<sprite>(m_garage, battlefield);
    }

    static void init(entt::registry &registry)
    {
        auto view = registry.view<sprite>();

        for (auto entity : view)
        {
            auto &entity_sprite = view.get<sprite>(entity);
            glUseProgram(entity_sprite._shader._program_id);
            initialize(entity_sprite._texture);
            glUseProgram(0);
        }

        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (auto entity : view)
        {
            auto const &ent_sprite = view.get<sprite>(entity);
            glUseProgram(ent_sprite._shader._program_id);

            glUniform1i(ent_sprite._shader.get_uniform_location("texture1"),
                        ent_sprite._texture._number);
            glUseProgram(0);
        }

        for (auto entity : view)
        {
            auto &ent_sprite = view.get<sprite>(entity);
            registry.emplace<imgui_sprite_editor>(entity, ent_sprite);
        }
    }

    struct sprite_comparator
    {
        entt::registry &registry;

        explicit sprite_comparator(entt::registry &registry)
            : registry(registry)
        {
        }

        bool operator()(const entt::entity lhs, const entt::entity rhs) const
        {
            const auto &lhs_sprite = registry.get<sprite>(lhs);
            const auto &rhs_sprite = registry.get<sprite>(rhs);
            return lhs_sprite._transform._position.z <
                   rhs_sprite._transform._position.z;
        }
    };

    void update(entt::registry &registry, entt::entity const &window_entity)
    {
        m_animation_system.update(registry);

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        const auto &sdl_context =
            registry.get<sdl_render_context>(window_entity);

        const float window_aspect_ratio =
            static_cast<float>(sdl_context.get_width()) /
            static_cast<float>(sdl_context.get_height());

        registry.sort<sprite>(sprite_comparator{registry});

        auto view = registry.view<sprite>();

        for (auto entity : view)
        {
            auto const &entity_sprite = view.get<sprite>(entity);

            entity_sprite.render_animated();
            entity_sprite.render();
        }

        for (auto entity : view)
        {
            auto const &entity_sprite = view.get<sprite>(entity);

            // Translate to origin
            const glm::mat4 projection_matrix =
                glm::ortho(-window_aspect_ratio, window_aspect_ratio, -1.0f,
                           1.0f, -1.0f, 1.0f);

            // Create the rotation matrix
            const glm::mat4 rotation_matrix1 =
                glm::rotate(glm::mat4(1.0f),
                            entity_sprite._transform._current_rotation_angle,
                            glm::vec3(0.0f, 0.0f, 1.0f));

            const glm::mat4 translation_matrix1 = glm::translate(
                glm::mat4(1.0f), entity_sprite._transform._position);

            const glm::mat4 scaling_matrix1 =
                glm::scale(glm::mat4(1.0f), entity_sprite._transform._scale);

            entity_sprite.apply_transform(translation_matrix1 *
                                          projection_matrix * rotation_matrix1 *
                                          scaling_matrix1);
        }
    } // namespace sdk

    static void destroy(entt::registry &registry)
    {
        auto view = registry.view<sprite>();

        for (auto entity : view)
        {
            auto const &entity_sprite = view.get<sprite>(entity);

            auto &texture = entity_sprite._texture;

            glDeleteVertexArrays(1, &texture._vertex_array_object);

            glDeleteBuffers(1, &texture._vertex_buffer_object);

            glDeleteBuffers(1, &texture._element_buffer_object);
        }
    }

private:
    static void initialize(opengl_texture &texture)
    {
        const auto png_data = get_png_data(texture);

        if (texture._width == 0 || texture._height == 0)
        {
            throw std::invalid_argument(
                "Given image bound are not determined. and Use get_png_data() "
                "before glTexImage2D.");
        }

        generate_buffers(texture);
        bind_buffers(texture);

        enable_attributes();
        glBindVertexArray(0);

        glGenTextures(1, &texture._texture);

        glBindTexture(GL_TEXTURE_2D, texture._texture);

        set_texture_parameters(texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture._width, texture._height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, png_data.data());

        if (texture._need_generate_mipmaps)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        std::cout << "Texture loaded: " << texture._image_path.string()
                  << std::endl;
    }

    [[nodiscard]] static std::vector<unsigned char>
    get_png_data(opengl_texture &texture)
    {
        std::string file_path_string = texture._image_path.string();
        if (!file_path_string.empty() && file_path_string[0] == '/')
        {
            file_path_string.erase(0, 1);
        }

        suppl::membuf file_contents = suppl::load_file(file_path_string);

        stbi_set_flip_vertically_on_load(true);
        int width                  = 0;
        int height                 = 0;
        int components             = 0;
        unsigned char *decoded_img = stbi_load_from_memory(
            reinterpret_cast<unsigned char *>(file_contents.begin()),
            file_contents.size(), &width, &height, &components, 4);

        if (!decoded_img)
        {
            throw std::invalid_argument("Failed to decode PNG image");
        }

        texture._width  = static_cast<unsigned long>(width);
        texture._height = static_cast<unsigned long>(height);

        std::vector<unsigned char> png_data(decoded_img,
                                            decoded_img + width * height * 4);
        stbi_image_free(decoded_img);

        if (png_data.empty())
        {
            throw std::invalid_argument("Empty PNG result");
        }

        return png_data;
    }

    static void generate_buffers(opengl_texture &texture)
    {
        glGenVertexArrays(1, &texture._vertex_array_object);

        glGenBuffers(1, &texture._vertex_buffer_object);

        glGenBuffers(1, &texture._element_buffer_object);
    }

    static void bind_buffers(opengl_texture &texture)
    {
        glBindVertexArray(texture._vertex_array_object);

        glBindBuffer(GL_ARRAY_BUFFER, texture._vertex_buffer_object);

        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(texture._vertices.size() * sizeof(GLfloat)),
            texture._vertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texture._element_buffer_object);

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(texture._indices.size() * sizeof(GLuint)),
            texture._indices.data(), GL_DYNAMIC_DRAW);
    }

    static void enable_attributes()
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              nullptr);

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              (GLvoid *)(3 * sizeof(GLfloat)));

        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              (GLvoid *)(6 * sizeof(GLfloat)));

        glEnableVertexAttribArray(2);
    }

    static void set_texture_parameters(opengl_texture const &texture)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

    entt::entity m_garage{};

    [[no_unique_address]] sprite_animation_system m_animation_system{};
};

} // namespace sdk
