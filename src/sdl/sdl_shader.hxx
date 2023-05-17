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

struct Vector2f
{
    Vector2f() : x(0.f), y(0.f) {}

    Vector2f(const GLfloat& x, const GLfloat y) : x(x), y(y) {}
    GLfloat x;
    GLfloat y;
};

struct Triangle2D
{
    Triangle2D()
    {
        v[0] = Vector2f();
        v[1] = Vector2f();
        v[2] = Vector2f();
    }

    Vector2f v[3];
};

class OpenGLShader : public AbstractEngine::IShader<OpenGLShader>
{
public:
    void initialize_impl(const std::filesystem::path& vertex_path,
                         const std::filesystem::path& fragment_path)
    {
        vertex_source_   = vertex_path;
        fragment_source_ = fragment_path;

        create_shader();
    }

    void create_shader() {
        const std::string vertex_file_content   = GL::get_file_content(vertex_source_);
        const std::string fragment_file_content = GL::get_file_content(fragment_source_);

        const GLchar* vertex_content   = vertex_file_content.data();
        const GLchar* fragment_content = fragment_file_content.data();


        compile_impl(vertex_content, fragment_content);
        link_impl();

        delete_shaders();

        generate_buffers();
        bind_buffer();
    }

    void reload_impl()
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

        create_shader();
    }

    void destroy_impl()
    {
        if (program_id_ == 0)
        {
            std::cerr << "Failed to destroy shader: The program id is 0";
            return;
        }

        glDeleteVertexArrays(1, &VAO_);
        GL::listen_opengl_errors();

        glDeleteBuffers(1, &VBO_);
        GL::listen_opengl_errors();

        glDeleteBuffers(1, &EBO_);
        GL::listen_opengl_errors();

        glDeleteProgram(program_id_);
        GL::listen_opengl_errors();
    }

    void render()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        GL::listen_opengl_errors();

        glClear(GL_COLOR_BUFFER_BIT);
        GL::listen_opengl_errors();

        glUseProgram(program_id_);
        GL::listen_opengl_errors();

        glBindVertexArray(VAO_);
        GL::listen_opengl_errors();

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        GL::listen_opengl_errors();

        glBindVertexArray(0);
        GL::listen_opengl_errors();
    }

private:
    void compile_impl(const GLchar* vertex_content, const GLchar* fragment_content)
    {
        vertex_shader_   = GL::compile_shader(GL_VERTEX_SHADER, vertex_content);
        fragment_shader_ = GL::compile_shader(GL_FRAGMENT_SHADER, fragment_content);
    }

    void link_impl()
    {
        program_id_ = GL::create_program();

        GL::attach_shader(program_id_, vertex_shader_);
        GL::attach_shader(program_id_, fragment_shader_);

        GL::link_shader_program(program_id_);
    }

    void delete_shaders()
    {
        GL::delete_shader(vertex_shader_);
        GL::delete_shader(fragment_shader_);
    }

    void generate_buffers()
    {
        GL::generate_vertex_array(1, &VAO_);
        GL::generate_buffer_object_name(1, &VBO_);
        GL::generate_buffer_object_name(1, &EBO_);
    }

    void bind_buffer()
    {
        glBindVertexArray(VAO_);
        GL::listen_opengl_errors();

        Triangle2D vertices;
        vertices.v[0]                = Vector2f{0.f, 0.2f};
        vertices.v[1]                = Vector2f{0.5f,  -0.5f};
        vertices.v[2]                = Vector2f{-0.5f, -0.5f};

        constexpr GLuint indices[] = {
            0, 1, 3, // First Triangle
            1, 0, 3  // Second Triangle
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        GL::listen_opengl_errors();

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.v), vertices.v, GL_STREAM_DRAW);
        GL::listen_opengl_errors();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
        GL::listen_opengl_errors();

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);
        GL::listen_opengl_errors();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        GL::listen_opengl_errors();

        glEnableVertexAttribArray(0);
        GL::listen_opengl_errors();

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        GL::listen_opengl_errors();

        glBindVertexArray(0);
        GL::listen_opengl_errors();
    }

    GLuint vertex_shader_;
    GLuint fragment_shader_;
    GLuint program_id_{};

    std::filesystem::path vertex_source_;
    std::filesystem::path fragment_source_;

    GLuint VBO_, VAO_, EBO_;
};

} // namespace SDL
