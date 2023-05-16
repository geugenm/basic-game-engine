#pragma once

#include <functional>
#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <render/shader.hxx>
#include <stdexcept>

#include "opengl_functions.hxx"

namespace SDL
{

class OpenGLShader : public AbstractEngine::IShader<OpenGLShader>
{
public:
    template <typename... Args>
    void initialize_impl(const std::filesystem::path& vertex_path,
                         const std::filesystem::path& fragment_path,
                         const std::vector<std::tuple<GLuint, const GLchar*>>& attributes,
                         Args&&... args)
    {
        const std::string vertex_content   = GL::get_file_content(vertex_path);
        const std::string fragment_content = GL::get_file_content(fragment_path);

        vertex_source_   = vertex_path;
        fragment_source_ = fragment_path;

        compile_impl(vertex_content.data(), fragment_content.data());
        link_impl();
        init_buffer();
    }

    template <typename... Args>
    void reload_impl(const std::vector<std::tuple<GLuint, const GLchar*>>& attributes,
                     Args&&... args)
    {
        if (!exists(vertex_source_))
        {
            throw std::invalid_argument("Can't reload shader, shader file was not found: " +
                                        vertex_source_.string());
        }

        if (!exists(fragment_source_))
        {
            throw std::invalid_argument("Can't reload shader, shader file was not found: " +
                                        fragment_source_.string());
        }

        initialize_impl(vertex_source_, fragment_source_, attributes);
    }

    template <typename... Args> void destroy_impl(Args&&... args)
    {
        if (program_id_ == 0)
        {
            std::cerr << "Failed to destroy shader: The program id is 0";
            return;
        }

        GL::destroy_shader(program_id_);
        glDeleteProgram(program_id_);
        GL::listen_opengl_errors();
    }

    template <typename... Args, typename T>
    void set_uniform_impl(const std::string& name, const T& value, Args&&... args)
    {
    }

    void use(GLfloat * vertices)
    {
        // Update the vertex buffer with the new vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        GL::listen_opengl_errors();
        glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_DYNAMIC_DRAW);
        GL::listen_opengl_errors();

        // Re-bind the VAO after updating the vertex buffer
        glBindVertexArray(VAO_);
        GL::listen_opengl_errors();

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)nullptr);
        GL::listen_opengl_errors();

        glEnableVertexAttribArray(0);
        GL::listen_opengl_errors();

        glBindVertexArray(0);
        GL::listen_opengl_errors();

        // Clear the screen and draw the new triangle
        glClear(GL_COLOR_BUFFER_BIT);
        GL::listen_opengl_errors();

        GL::use_shader(program_id_);

        glBindVertexArray(VAO_);
        GL::listen_opengl_errors();

        glDrawArrays(GL_TRIANGLES, 0, 3);
        GL::listen_opengl_errors();

        glBindVertexArray(0);
        GL::listen_opengl_errors();
    }

    void init_buffer() {
        glGenVertexArrays(1, &VAO_);
        GL::listen_opengl_errors();

        glGenBuffers(1, &VBO_);
        GL::listen_opengl_errors();

        glBindVertexArray(VAO_);
        GL::listen_opengl_errors();

        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        GL::listen_opengl_errors();

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)nullptr);
        GL::listen_opengl_errors();

        glEnableVertexAttribArray(0);
        GL::listen_opengl_errors();

        glBindVertexArray(0);
        GL::listen_opengl_errors();
    }

private:
    template <typename... Args>
    void compile_impl(const char* vertex_content, const char* fragment_content, Args&&... args)
    {
        vertex_shader_   = GL::compile_shader(GL_VERTEX_SHADER, vertex_content);
        fragment_shader_ = GL::compile_shader(GL_FRAGMENT_SHADER, fragment_content);
    }

    template <typename... Args>
    void link_impl(Args&&... args)
    {
        program_id_ = GL::link_shader_program(vertex_shader_);
        program_id_ = GL::link_shader_program(fragment_shader_);
    }

    GLuint vertex_shader_{};
    GLuint fragment_shader_{};
    GLuint program_id_{};

    std::filesystem::path vertex_source_;
    std::filesystem::path fragment_source_;

    GLuint VBO_, VAO_, UBO_;
};

} // namespace SDL
