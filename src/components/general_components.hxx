#pragma once

#include "../util/opengl/glad/glad.h"

#include <filesystem>
#include <glm/glm.hpp>
#include <vector>

namespace sdk
{
struct opengl_shader
{
    std::filesystem::path _vertex_source_path;
    std::filesystem::path _fragment_source_path;

    GLuint _program_id{};

    [[nodiscard]] GLint get_uniform_location(const GLchar *uniform_name) const
    {
        const GLint uniform_location =
            glGetUniformLocation(_program_id, uniform_name);

        if (uniform_location == -1)
        {
            throw std::invalid_argument("Given uniform '" +
                                        std::string(uniform_name) +
                                        "' is not found.");
        }

        return uniform_location;
    }
};

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

    bool _need_generate_mipmaps = true;
    bool _needs_to_be_scaled    = true;

    GLint _number{};

    [[nodiscard]] float get_image_aspect_ratio() const
    {
        return static_cast<float>(_width) / static_cast<float>(_height);
    }
};

struct texture_params
{
    glm::vec2 position{0.5f, -0.5f};
    float rotationAngle = 0.0f;

    const float moveSpeed   = 0.03f;
    const float rotateSpeed = 0.02f;

    float halfWidth  = 0.6f * 0.5f;
    float halfHeight = 0.6f * 0.5f;
};

struct transform
{
    glm::vec2 _position{0.5f, -0.5f};
    float _rotation_angle = 0.0f;

    const float _move_speed;
    const float _rotate_speed;
};

struct sprite
{
    opengl_shader _shader;
    opengl_texture _texture;

    transform _transform;
};

struct player_tank
{
    sprite _hull_sprite;
    sprite _turret_sprite;
};
} // namespace sdk