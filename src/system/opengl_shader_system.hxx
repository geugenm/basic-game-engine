#pragma once

#include "entt/entt.hpp"
#include "opengl_functions.hxx"

namespace sdk
{

struct opengl_shader
{
    std::filesystem::path _vertex_source_path;
    std::filesystem::path _fragment_source_path;

    GLuint _program_id{};

    bool _is_initialized = false;

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

struct example_triangle_shader
{
    opengl_shader _shader{
        ._vertex_source_path   = "../resources/shaders/triangle.vert",
        ._fragment_source_path = "../resources/shaders/triangle.frag",
    };
};

class opengl_shader_system
{
public:
    void example_shader(entt::registry &registry)
    {
        const auto entity = registry.create();
        example_triangle_shader triangle_shader;
        create_shader_program(triangle_shader._shader);
        registry.emplace<example_triangle_shader>(entity, triangle_shader);
    }

    void init(entt::registry &registry)
    {
        auto view = registry.view<opengl_shader>();

        for (auto entity : view)
        {
            auto &shader = view.get<opengl_shader>(entity);
            create_shader_program(shader);
        }
    }

    void update(entt::registry &registry)
    {
        auto view = registry.view<example_triangle_shader>();

        for (auto entity : view)
        {
            auto &shader = view.get<example_triangle_shader>(entity);
            use(shader._shader);
        }
    }

private:
    static void create_shader_program(opengl_shader &shader)
    {
        opengl_subsdk::enable_debug_mode();
        if (!exists(shader._vertex_source_path))
        {
            throw std::invalid_argument("Vertex shader file '" +
                                        shader._vertex_source_path.string() +
                                        "' is not found.");
        }

        if (!exists(shader._fragment_source_path))
        {
            throw std::invalid_argument("Fragment shader file '" +
                                        shader._fragment_source_path.string() +
                                        "' is not found.");
        }

        const std::string vertex_source = shader._vertex_source_path.string();
        const std::string fragment_source =
            shader._fragment_source_path.string();

        GLuint vertex = opengl_subsdk::get_compiled_shader_from_file(
            GL_VERTEX_SHADER, vertex_source.data());
        GLuint fragment = opengl_subsdk::get_compiled_shader_from_file(
            GL_FRAGMENT_SHADER, fragment_source.data());

        shader._program_id = opengl_subsdk::get_new_program();

        opengl_subsdk::attach_shader(shader._program_id, vertex);
        opengl_subsdk::attach_shader(shader._program_id, fragment);

        opengl_subsdk::link_shader_program(shader._program_id);

        opengl_subsdk::delete_shader(vertex);
        opengl_subsdk::delete_shader(fragment);

        shader._is_initialized = true;
        opengl_subsdk::disable_debug_mode();
    }

    static void use(opengl_shader &shader)
    {
        if (shader._is_initialized == false)
        {
            throw std::invalid_argument("Shader is not initialized.");
        }
        glUseProgram(shader._program_id);
    }
};

} // namespace sdk
