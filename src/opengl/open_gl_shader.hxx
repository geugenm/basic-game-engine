#pragma once

#include "opengl_functions.hxx"

#include <fstream>
#include <iostream>

namespace opengl_subsdk
{

class shader
{
public:
    explicit shader(const char *vertex_path, const char *fragment_path)
        : vertex_source_path_(vertex_path), fragment_source_path_(fragment_path)
    {
        if (!vertex_source_path_ || !fragment_source_path_) {
            throw std::invalid_argument("Vertex and fragment shader paths are empty.");
        }
        create_shader_program();
    }

    void recompile()
    {
        create_shader_program();
    }

    void use() const
    {
        glUseProgram(program_id_);
    }

    [[nodiscard]] GLuint get_program_id() const
    {
        return program_id_;
    }

    [[nodiscard]] GLint get_uniform_location(const char *uniform_name) const
    {
        const GLint uniform_location =
            glGetUniformLocation(get_program_id(), uniform_name);

        if (uniform_location == -1)
        {
            throw std::invalid_argument("Given uniform is not found.");
        }

        return uniform_location;
    }

    ~shader() = default;

private:
    void create_shader_program()
    {
        GLuint vertex = opengl_subsdk::get_compiled_shader_from_file(
            GL_VERTEX_SHADER, vertex_source_path_);
        GLuint fragment = opengl_subsdk::get_compiled_shader_from_file(
            GL_FRAGMENT_SHADER, fragment_source_path_);

        program_id_ = opengl_subsdk::get_new_program();

        opengl_subsdk::attach_shader(program_id_, vertex);
        opengl_subsdk::attach_shader(program_id_, fragment);

        opengl_subsdk::link_shader_program(program_id_);

        opengl_subsdk::delete_shader(vertex);
        opengl_subsdk::delete_shader(fragment);
    }

    GLuint program_id_{};

    const char *vertex_source_path_;
    const char *fragment_source_path_;
};

} // namespace opengl_subsdk
