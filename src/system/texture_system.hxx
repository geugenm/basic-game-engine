#pragma once

#include <SDL_events.h>
#include <SDL_mouse.h>

#include "sdl_render_system.hxx"
#include <general_components.hxx>

#include <nlohmann/json_fwd.hpp>
#include <ostream>
#include <render/picopng.hxx>

#include <entt/entt.hpp>
#include <glm/fwd.hpp>
#include <opengl_functions.hxx>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/vector_angle.hpp>
#include <nlohmann/json.hpp>
#include <stdexcept>

namespace sdk
{

struct sprite_animator
{
    std::size_t _current_frame;
    std::size_t _rows;
    std::size_t _columns;

    // TODO: Make it handling sprites directly not just texture coordinates
    void sprite_animation_update(float *texture_coordinates) const
    {
        if (texture_coordinates == nullptr)
        {
            throw std::invalid_argument(
                "Can't update frame: `texture_coordinates` is null.");
        }

        const int X = 0;
        const int Y = 1;

        const int V0 = 0;
        const int V1 = 2;
        const int V2 = 4;
        const int V3 = 6;

        const float frameWidth  = 1.f / static_cast<float>(_columns);
        const float frameHeight = 1.f / static_cast<float>(_rows);

        const auto floated_rows =
            static_cast<float>(_rows - _current_frame / _columns);
        const auto floated_columns =
            static_cast<float>(_current_frame % _columns);

        texture_coordinates[V0 + X] = frameWidth * floated_columns;
        texture_coordinates[V0 + Y] = frameHeight * floated_rows;

        texture_coordinates[V1 + X] = frameWidth * (floated_columns + 1);
        texture_coordinates[V1 + Y] = frameHeight * floated_rows;

        texture_coordinates[V2 + X] = frameWidth * (floated_columns + 1);
        texture_coordinates[V2 + Y] = frameHeight * (floated_rows + 1);

        texture_coordinates[V3 + X] = frameWidth * floated_columns;
        texture_coordinates[V3 + Y] = frameHeight * (floated_rows + 1);
    }

    [[nodiscard]] sprite_animator init_new_animator(const std::size_t &rows,
                                                    const std::size_t &cols,
                                                    float *texture_coordinates)
    {
        // TODO: Handle texture coordinates verify according to size given
        sprite_animator animator{
            ._current_frame = 0,
            ._rows          = rows,
            ._columns       = cols,
        };

        if (texture_coordinates == nullptr)
        {
            throw std::invalid_argument(
                "Failed to init animator: `texture_coordinates` is null.");
        }

        sprite_animation_update(texture_coordinates);

        return animator;
    }

    void sprite_animation_next_frame(float *texture_coordinates)
    {
        const auto max_frame = _columns * _rows - 1;

        if (max_frame == _current_frame)
        {
            _current_frame = 0;
        }
        else
        {
            _current_frame++;
        }

        sprite_animation_update(texture_coordinates);
    }
};

class opengl_texture_system final
{
public:
    entt::entity m_garage;
    entt::entity m_chair;
    entt::entity m_body;
    entt::entity m_head;
    entt::entity m_pants;
    entt::entity m_hands;

    sprite_animator m_sprite_animator{};

    void test(entt::registry &registry)
    {
        m_garage = registry.create();
        m_body   = registry.create();
        m_chair  = registry.create();
        m_head   = registry.create();
        m_pants  = registry.create();
        m_hands  = registry.create();

        sprite battlefield = sprite::get_sprite_from_file("level1");

        sprite chair = sprite::get_sprite_from_file("chair");

        sprite body = sprite::get_sprite_from_file("body");

        sprite head = sprite::get_sprite_from_file("head");

        sprite pants = sprite::get_sprite_from_file("pants");

        sprite hands = sprite::get_sprite_from_file("hands");

        registry.emplace<sprite>(m_garage, battlefield);
        registry.emplace<sprite>(m_chair, chair);
        registry.emplace<sprite>(m_body, body);
        registry.emplace<sprite>(m_head, head);
        registry.emplace<sprite>(m_pants, pants);
        registry.emplace<sprite>(m_hands, hands);
    }

    void init_on(entt::registry &registry, entt::entity const &window_entity)
    {
        auto sdl_context = registry.get<sdl_render_context>(window_entity);

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

        for (auto entity : view)
        {
            auto const &ent_sprite = view.get<sprite>(entity);
            glUseProgram(ent_sprite._shader._program_id);

            glUniform1i(ent_sprite._shader.get_uniform_location("texture"),
                        ent_sprite._texture._number);
            glUseProgram(0);
        }

        { // Init sprite animator
            // TODO: take all this parameters from sprite
            auto &player_hands_vertices =
                registry.get<sprite>(m_hands)._texture._vertices;

            const auto sprite_rows = 4;
            const auto sprite_cols = 4;
            m_sprite_animator      = m_sprite_animator.init_new_animator(
                sprite_rows, sprite_cols, player_hands_vertices.data());
        }
    }

    void handle_events(entt::registry &registry, const SDL_Event &event)
    {
        // TODO: implement frame time in order to fix blazing speed ups
    }

    void update(entt::registry &registry, entt::entity const &window_entity)
    {
        auto const &battlefield_sprite = registry.get<sprite>(m_garage);
        auto const &chair_sprite       = registry.get<sprite>(m_chair);
        auto const &body_sprite        = registry.get<sprite>(m_body);
        auto const &head_sprite        = registry.get<sprite>(m_head);
        auto const &pants_sprite       = registry.get<sprite>(m_pants);
        auto const &hands_sprite       = registry.get<sprite>(m_hands);

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        battlefield_sprite.render();
        chair_sprite.render();
        pants_sprite.render();
        hands_sprite.render();
        body_sprite.render();
        head_sprite.render();

        const auto sdl_context =
            registry.get<sdl_render_context>(window_entity);

        const float window_aspect_ratio =
            static_cast<float>(sdl_context.get_width()) /
            static_cast<float>(sdl_context.get_height());

        const float texture_aspect =
            battlefield_sprite._texture.get_image_aspect_ratio();

        float left;
        float right;

        float scale = window_aspect_ratio / texture_aspect;

        left  = -texture_aspect * scale;
        right = texture_aspect * scale;

        const glm::mat4 projection_matrix =
            glm::ortho(left, right, -1.0f, 1.0f, -1.0f, 1.0f);

        const auto transform = projection_matrix;

        const glm::mat4 scaling_matrix =
            glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 1.0f));
        const glm::mat4 translation_matrix =
            glm::translate(glm::mat4(1.0f), glm::vec3(-0.2f, 0.1f, 0.0f));

        const auto final_transform =
            projection_matrix * translation_matrix * scaling_matrix;

        battlefield_sprite.apply_transform(transform);
        chair_sprite.apply_transform(final_transform);
        body_sprite.apply_transform(final_transform);
        head_sprite.apply_transform(final_transform);
        pants_sprite.apply_transform(final_transform);
        hands_sprite.apply_transform(final_transform);
    } // namespace sdk

private:
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
        bind_buffers(texture);

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

        std::cout << "Texture loaded: " << texture._image_path.string()
                  << std::endl;
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

    static void bind_buffers(opengl_texture &texture)
    {
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
};

} // namespace sdk
