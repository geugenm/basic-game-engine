#pragma once

#include "render/picopng.hxx"

#include <entt/entt.hpp>
#include <opengl_functions.hxx>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sdl_render_engine.hxx"

namespace sdk
{

struct opengl_texture
{
    std::filesystem::path _image_path;

    GLuint _VBO{};
    GLuint _VAO{};
    GLuint _EBO{};

    GLuint _texture{};

    unsigned long _width{};
    unsigned long _height{};

    std::vector<GLfloat> _vertices;
    std::vector<GLuint> _indices;

    bool _is_initialized = false;

    GLint _number{};

    sdl_render_context _attached_context;

    [[nodiscard]] float image_aspect_ratio() const
    {
        return static_cast<float>(_width) / static_cast<float>(_height);
    }
};

struct texture_params
{
    glm::vec2 position{0.5f, -0.5f};
    float rotationAngle = 0.0f;

    const float moveSpeed   = 0.05f;
    const float rotateSpeed = 0.05f;

    float halfWidth  = 0.6f * 0.5f;
    float halfHeight = 0.6f * 0.5f;
};

struct opengl_texture_system
{
    entt::entity _tank_hull;
    entt::entity _tank_turret;

    void test(entt::registry &registry)
    {
        _tank_hull   = registry.create();
        _tank_turret = registry.create();

        opengl_shader shader{
            ._vertex_source_path   = "../resources/shaders/texture.vert",
            ._fragment_source_path = "../resources/shaders/texture.frag",
        };

        opengl_texture hull_texture{
            ._image_path = "../resources/textures/hull.png",
            // clang-format off
            ._vertices = {
                // Positions          // Colors           // Texture Coords
                0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
                0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
                -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f  // Top Left
            },

            ._indices = {
                // Note that we start from 0!
                0, 1, 3, // First Triangle
                1, 2, 3  // Second Triangle
            },
            // clang-format on
            ._number = 0,
        };

        opengl_texture turret_texture{
            ._image_path = "../resources/textures/turret.png",
            // clang-format off
            ._vertices = {
                // Positions          // Colors           // Texture Coords
                0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
                0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
                -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f  // Top Left
            },

            ._indices = {
                // Note that we start from 0!
                0, 1, 3, // First Triangle
                1, 2, 3  // Second Triangle
            },
            // clang-format on
            ._number = 1,
        };

        registry.emplace<opengl_shader>(_tank_hull, shader);

        registry.emplace<opengl_texture>(_tank_hull, hull_texture);

        registry.emplace<opengl_texture>(_tank_turret, turret_texture);
    }

    void init_on(entt::registry &registry, entt::entity &window_entity)
    {

        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        auto view_context = registry.view<sdl_render_context>();
        auto &sdl_context = view_context.get<sdl_render_context>(window_entity);

        auto view = registry.view<opengl_texture>();

        for (auto entity : view)
        {
            auto &texture = view.get<opengl_texture>(entity);
            initialize(texture, sdl_context);
        }

        auto &shader =
            registry.view<opengl_shader>().get<opengl_shader>(_tank_hull);
        glUseProgram(shader._program_id);

        {
            const auto &tank_hull_texture =
                view.get<opengl_texture>(_tank_hull);

            LOG(INFO) << "Tank hull texture number: "
                      << tank_hull_texture._number;

            glUniform1i(shader.get_uniform_location("ourTexture"),
                        tank_hull_texture._number);
        }

        {
            const auto &tank_turret_texture =
                view.get<opengl_texture>(_tank_turret);

            LOG(INFO) << "Tank turret texture number: "
                      << tank_turret_texture._number;

            glUniform1i(shader.get_uniform_location("topTexture"),
                        tank_turret_texture._number);
        }
    }

    void update(entt::registry &registry, entt::entity &window_entity)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        auto view = registry.view<opengl_texture>();

        for (auto entity : view)
        {
            auto &texture = view.get<opengl_texture>(entity);
            GLenum target = GL_TEXTURE0 + texture._texture;
            glActiveTexture(target);
            render(texture);
        }

        auto event_view = registry.view<sdk::keyboard>();

        auto view_context = registry.view<sdl_render_context>();
        auto &sdl_context = view_context.get<sdl_render_context>(window_entity);

        const float aspect_ratio = static_cast<float>(sdl_context.get_width()) /
                                   static_cast<float>(sdl_context.get_height());
        glm::mat4 aspect_matrix =
            glm::scale(glm::mat4(1.0f), glm::vec3(aspect_ratio, 1.0f, 1.0f));

        static texture_params params;

        for (auto entity : event_view)
        {
            auto &keyboard = event_view.get<sdk::keyboard>(entity);
            switch (keyboard)
            {
                case sdk::keyboard::keyboard_W:
                    params.position.x +=
                        params.moveSpeed * cos(params.rotationAngle);
                    params.position.y +=
                        params.moveSpeed * sin(params.rotationAngle);
                    params.position.x =
                        std::clamp(params.position.x, -1.0f + params.halfWidth,
                                   1.0f - params.halfWidth);
                    params.position.y =
                        std::clamp(params.position.y, -1.0f + params.halfHeight,
                                   1.0f - params.halfHeight);
                    break;
                case sdk::keyboard::keyboard_S:
                    params.position.x -=
                        params.moveSpeed * cos(params.rotationAngle);
                    params.position.y -=
                        params.moveSpeed * sin(params.rotationAngle);
                    params.position.x =
                        std::clamp(params.position.x, -1.0f + params.halfWidth,
                                   1.0f - params.halfWidth);
                    params.position.y =
                        std::clamp(params.position.y, -1.0f + params.halfHeight,
                                   1.0f - params.halfHeight);
                    break;
                case sdk::keyboard::keyboard_A:
                    params.rotationAngle += params.rotateSpeed;
                    break;
                case sdk::keyboard::keyboard_D:
                    params.rotationAngle -= params.rotateSpeed;
                    break;
                default:
                    break;
            }
            registry.destroy(entity);
        }

        {
            auto transform = glm::mat4(1.0f);
            transform      = glm::scale(transform, glm::vec3(0.6f, 0.6f, 0.6f));
            transform =
                glm::translate(transform, glm::vec3(params.position.x,
                                                    params.position.y, 0.0f));
            transform = glm::rotate(transform, params.rotationAngle,
                                    glm::vec3(0.0f, 0.0f, 1.0f));
            transform = transform * aspect_matrix;

            auto &shader =
                registry.view<opengl_shader>().get<opengl_shader>(_tank_hull);

            glUniformMatrix4fv(shader.get_uniform_location("transform"), 1,
                               GL_FALSE, glm::value_ptr(transform));
        }
    }

private:
    void render(opengl_texture &texture)
    {
        glBindTexture(GL_TEXTURE_2D, texture._texture);

        glBindVertexArray(texture._VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
    }

    void destroy(opengl_texture &texture)
    {
        glDeleteVertexArrays(1, &texture._VAO);

        glDeleteBuffers(1, &texture._VBO);

        glDeleteBuffers(1, &texture._EBO);
    }

    void initialize(opengl_texture &texture, const sdl_render_context &context)
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

        set_texture_parameters();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture._width, texture._height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, png_data.data());

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        texture._is_initialized = true;
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

    void generate_buffers(opengl_texture &texture)
    {
        glGenVertexArrays(1, &texture._VAO);

        glGenBuffers(1, &texture._VBO);

        glGenBuffers(1, &texture._EBO);
    }

    void bind_buffers(opengl_texture &texture,
                      const sdl_render_context &sdl_context)
    {
        float image_aspect_ratio = texture.image_aspect_ratio();
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

        glBindVertexArray(texture._VAO);

        glBindBuffer(GL_ARRAY_BUFFER, texture._VBO);

        glBufferData(GL_ARRAY_BUFFER,
                     texture._vertices.size() * sizeof(GLfloat),
                     texture._vertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texture._EBO);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     texture._indices.size() * sizeof(GLuint),
                     texture._indices.data(), GL_DYNAMIC_DRAW);
    }

    void enable_attributes()
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

    static void set_texture_parameters()
    {
        // Set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
};

} // namespace sdk
