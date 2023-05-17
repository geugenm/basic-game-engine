#pragma once

#include <fstream>
#include <functional>
#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <render/shader.hxx>
#include <stdexcept>

#include "render/picopng.hxx"

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
        _vertices[0] = Vector2f();
        _vertices[1] = Vector2f();
        _vertices[2] = Vector2f();
    }

    Vector2f _vertices[3];
};

template <typename Derived>
class OpenGLShader : public AbstractEngine::IShader<OpenGLShader<Derived>>
{
public:
    void initialize_impl(const std::filesystem::path& vertex_path,
                         const std::filesystem::path& fragment_path)
    {
        vertex_shader_path_  = vertex_path;
        fragment_shader_path = fragment_path;

        compile_and_link();
    }

    void compile_and_link()
    {
        const std::string vertex_file_content   = GL::get_file_content(vertex_shader_path_);
        const std::string fragment_file_content = GL::get_file_content(fragment_shader_path);

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
        if (!exists(vertex_shader_path_))
        {
            throw std::invalid_argument("Can't reload shader, shader file was not found: " +
                                        vertex_shader_path_.string());
        }

        if (!exists(fragment_shader_path))
        {
            throw std::invalid_argument("Can't reload shader, shader file was not found: " +
                                        fragment_shader_path.string());
        }

        compile_and_link();
    }

    void destroy_impl()
    {
        if (shader_program_id_ == 0)
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

        glDeleteProgram(shader_program_id_);
        GL::listen_opengl_errors();
    }

    template <typename... Args> void render(Args&&... args)
    {
        static_cast<Derived*>(this)->render_impl(std::forward<Args>(args)...);
    }

protected:
    void compile_impl(const GLchar* vertex_content, const GLchar* fragment_content)
    {
        vertex_shader_id_   = GL::compile_shader(GL_VERTEX_SHADER, vertex_content);
        fragment_shader_id_ = GL::compile_shader(GL_FRAGMENT_SHADER, fragment_content);
    }

    void link_impl()
    {
        shader_program_id_ = GL::create_program();

        GL::attach_shader(shader_program_id_, vertex_shader_id_);
        GL::attach_shader(shader_program_id_, fragment_shader_id_);

        GL::link_shader_program(shader_program_id_);
    }

    void delete_shaders()
    {
        GL::delete_shader(vertex_shader_id_);
        GL::delete_shader(fragment_shader_id_);
    }

    void generate_buffers()
    {
        GL::generate_vertex_array(1, &VAO_);
        GL::generate_buffer_object_name(1, &VBO_);
        GL::generate_buffer_object_name(1, &EBO_);
    }

    template <typename... Args> void bind_buffer(Args&&... args)
    {
        static_cast<Derived*>(this)->bind_buffer_impl(std::forward<Args>(args)...);
    }

    GLuint vertex_shader_id_;
    GLuint fragment_shader_id_;
    GLuint shader_program_id_{};

    std::filesystem::path vertex_shader_path_;
    std::filesystem::path fragment_shader_path;

    GLuint VBO_, VAO_, EBO_;
};

class SomeShader : public OpenGLShader<SomeShader>
{
public:
    void bind_buffer_impl()
    {
        glBindVertexArray(VAO_);
        GL::listen_opengl_errors();

        Triangle2D vertices;
        vertices._vertices[0] = Vector2f{0.f, 0.2f};
        vertices._vertices[1] = Vector2f{0.5f, -0.5f};
        vertices._vertices[2] = Vector2f{-0.5f, -0.5f};

        constexpr GLuint indices[] = {
            0, 1, 3, // First Triangle
            1, 0, 3  // Second Triangle
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        GL::listen_opengl_errors();

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices._vertices), vertices._vertices,
                     GL_STREAM_DRAW);
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

    void render_impl()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        GL::listen_opengl_errors();

        glClear(GL_COLOR_BUFFER_BIT);
        GL::listen_opengl_errors();

        glUseProgram(shader_program_id_);
        GL::listen_opengl_errors();

        glBindVertexArray(VAO_);
        GL::listen_opengl_errors();

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        GL::listen_opengl_errors();

        glBindVertexArray(0);
        GL::listen_opengl_errors();
    }
};

} // namespace SDL
