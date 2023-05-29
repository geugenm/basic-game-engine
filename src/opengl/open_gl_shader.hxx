#pragma once

#include "opengl_functions.hxx"

#include <fstream>
#include <iostream>

namespace OpenGLWrapper
{

class Shader
{
public:
    explicit Shader(const char *vertex_path, const char *fragment_path)
        : vertex_source_path_(vertex_path), fragment_source_path_(fragment_path)
    {
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

    ~Shader() = default;

private:
    void create_shader_program()
    {
        GLuint vertex = OpenGLWrapper::get_compiled_shader_from_file(
            GL_VERTEX_SHADER, vertex_source_path_);
        GLuint fragment = OpenGLWrapper::get_compiled_shader_from_file(
            GL_FRAGMENT_SHADER, fragment_source_path_);

        program_id_ = OpenGLWrapper::get_new_program();

        OpenGLWrapper::attach_shader(program_id_, vertex);
        OpenGLWrapper::attach_shader(program_id_, fragment);

        OpenGLWrapper::link_shader_program(program_id_);

        OpenGLWrapper::delete_shader(vertex);
        OpenGLWrapper::delete_shader(fragment);
    }

    GLuint program_id_{};

    const char *vertex_source_path_;
    const char *fragment_source_path_;
};

} // namespace OpenGLWrapper
