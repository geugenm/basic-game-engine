#pragma once

#include <SDL_events.h>
#include <SDL_mouse.h>

#include "sdl_render_system.hxx"
#include <general_components.hxx>

#include <nlohmann/json_fwd.hpp>
#include <ostream>

#include <entt/entt.hpp>
#include <glm/fwd.hpp>
#include <opengl_functions.hxx>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/vector_angle.hpp>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include <stb_image.h>

namespace sdk
{

struct sprite_animator final
{
    std::size_t _current_frame;
    std::size_t _rows;
    std::size_t _columns;

    // TODO: Make it handling sprites directly not just texture coordinates
    void update(sprite &sprite) const
    {
        if (sprite._texture._vertices.empty())
        {
            throw std::invalid_argument(
                "Can't update frame: `vertices` is empty.");
        }

        const int X = 0;
        const int Y = 1;

        const float frameWidth  = 1.f / static_cast<float>(_columns);
        const float frameHeight = 1.f / static_cast<float>(_rows);

        auto floated_rows =
            static_cast<float>(_rows - _current_frame / _columns);
        auto floated_columns = static_cast<float>(_current_frame % _columns);

        // Handle one frame case
        if (_columns == 1 && _rows == 1)
        {
            std::cout << "WARNING: No frames, just a sprite!" << std::endl;
            floated_rows    = 0.0f;
            floated_columns = 0.0f;
        }

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

    [[nodiscard]] static sprite_animator
    init_new_animator(const std::size_t &rows, const std::size_t &cols)
    {
        // TODO: Handle texture coordinates verify according to size given
        sprite_animator animator{
            ._current_frame = 0,
            ._rows          = rows,
            ._columns       = cols,
        };

        return animator;
    }

    void next_frame(sprite &sprite)
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

        std::cout << "Frame: " << _current_frame << std::endl;

        update(sprite);
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

        { // Init sprite animator
            const auto sprite_rows = 1;
            const auto sprite_cols = 1;
            m_sprite_animator =
                sprite_animator::init_new_animator(sprite_rows, sprite_cols);
        }

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
    }

    void handle_events(entt::registry &registry, const SDL_Event &event) const
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

        {
            // m_sprite_animator.next_frame(hands_sprite);
        }

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        battlefield_sprite.render();
        //        chair_sprite.render();
        //        pants_sprite.render();
        //        body_sprite.render();
        //        head_sprite.render();

        hands_sprite.render();

        const auto sdl_context =
            registry.get<sdl_render_context>(window_entity);

        const float window_aspect_ratio =
            static_cast<float>(sdl_context.get_width()) /
            static_cast<float>(sdl_context.get_height());

        const float texture_aspect =
            battlefield_sprite._texture.get_image_aspect_ratio();

        float scale = window_aspect_ratio / texture_aspect;

        const glm::mat4 projection_matrix = glm::ortho(
            -texture_aspect, texture_aspect, -1.0f, 1.0f, -1.0f, 1.0f);

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
        if (!std::filesystem::exists(texture._image_path))
        {
            throw std::invalid_argument("Failed to open PNG image: " +
                                        texture._image_path.string());
        }

        int width;
        int height;
        int channels;
        stbi_set_flip_vertically_on_load(1);
        unsigned char *data = stbi_load(texture._image_path.c_str(), &width,
                                        &height, &channels, STBI_rgb_alpha);

        if (!data)
        {
            throw std::invalid_argument("Failed to decode PNG image");
        }

        texture._width  = static_cast<unsigned long>(width);
        texture._height = static_cast<unsigned long>(height);

        std::vector<unsigned char> png_data(data, data + width * height * 4);
        stbi_image_free(data);

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
