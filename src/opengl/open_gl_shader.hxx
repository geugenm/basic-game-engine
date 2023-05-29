#pragma once

#include "opengl_functions.hxx"

#include <fstream>
#include <iostream>

namespace OpenGLWrapper
{

class Shader
{
public:
    Shader(const GLchar *vertex_path, const GLchar *fragment_path)
    {
        GLuint vertex = OpenGLWrapper::get_compiled_shader_from_file(GL_VERTEX_SHADER,
                                                                       vertex_path);
        GLuint fragment = OpenGLWrapper::get_compiled_shader_from_file(
            GL_FRAGMENT_SHADER, fragment_path);

        program_id_ = OpenGLWrapper::get_new_program();

        OpenGLWrapper::attach_shader(program_id_, vertex);
        OpenGLWrapper::attach_shader(program_id_, fragment);

        OpenGLWrapper::link_shader_program(program_id_);

        OpenGLWrapper::delete_shader(vertex);
        OpenGLWrapper::delete_shader(fragment);
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
    GLuint program_id_;
};

} // namespace OpenGLWrapper
