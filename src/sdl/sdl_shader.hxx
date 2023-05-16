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
                         const std::filesystem::path& fragment_path, Args&&... args)
    {
        const std::string vertex_file_content   = GL::get_file_content(vertex_path);
        const std::string fragment_file_content = GL::get_file_content(fragment_path);

        const GLchar* vertex_content   = vertex_file_content.data();
        const GLchar* fragment_content = fragment_file_content.data();

        vertex_source_   = vertex_path;
        fragment_source_ = fragment_path;

        vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
        GL::listen_opengl_errors();
        glShaderSource(vertex_shader_, 1, &vertex_content, nullptr);
        GL::listen_opengl_errors();

        glCompileShader(vertex_shader_);
        GL::listen_opengl_errors();


        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertex_shader_, GL_COMPILE_STATUS, &success);
        GL::listen_opengl_errors();

        if (!success)
        {
            glGetShaderInfoLog(vertex_shader_, 512, nullptr, infoLog);
            GL::listen_opengl_errors();

            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // Fragment shader
        fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
        GL::listen_opengl_errors();

        glShaderSource(fragment_shader_, 1, &fragment_content, nullptr);
        GL::listen_opengl_errors();

        glCompileShader(fragment_shader_);
        GL::listen_opengl_errors();

        // Check for compile time errors
        glGetShaderiv(fragment_shader_, GL_COMPILE_STATUS, &success);
        GL::listen_opengl_errors();

        if (!success)
        {
            glGetShaderInfoLog(fragment_shader_, 512, nullptr, infoLog);
            GL::listen_opengl_errors();

            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // Link shaders
        program_id_ = glCreateProgram();
        GL::listen_opengl_errors();

        glAttachShader(program_id_, vertex_shader_);
        GL::listen_opengl_errors();

        glAttachShader(program_id_, fragment_shader_);
        GL::listen_opengl_errors();

        glLinkProgram(program_id_);
        GL::listen_opengl_errors();

        // Check for linking errors
        glGetProgramiv(program_id_, GL_LINK_STATUS, &success);
        GL::listen_opengl_errors();

        if (!success)
        {
            glGetProgramInfoLog(program_id_, 512, nullptr, infoLog);
            GL::listen_opengl_errors();

            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(vertex_shader_);
        GL::listen_opengl_errors();

        glDeleteShader(fragment_shader_);
        GL::listen_opengl_errors();


        glGenVertexArrays(1, &VAO_);
        GL::listen_opengl_errors();

        glGenBuffers(1, &VBO_);
        GL::listen_opengl_errors();

        glGenBuffers(1, &EBO_);
        GL::listen_opengl_errors();

        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute
        // pointer(s).
        glBindVertexArray(VAO_);
        GL::listen_opengl_errors();


        GLfloat vertices[] = {
            0.5f,  0.5f,  0.0f, // Top Right
            0.5f,  -0.5f, 0.0f, // Bottom Right
            -0.5f, -0.5f, 0.0f, // Bottom Left
            -0.5f, 0.5f,  0.0f  // Top Left
        };
        GLuint indices[] = {
            // Note that we start from 0!
            0, 1, 3, // First Triangle
            1, 2, 3  // Second Triangle
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        GL::listen_opengl_errors();

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        GL::listen_opengl_errors();


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
        GL::listen_opengl_errors();

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        GL::listen_opengl_errors();


        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        GL::listen_opengl_errors();

        glEnableVertexAttribArray(0);
        GL::listen_opengl_errors();


        glBindBuffer(
            GL_ARRAY_BUFFER,
            0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as
                // the currently bound vertex buffer object so afterwards we can safely unbind

        GL::listen_opengl_errors();

        glBindVertexArray(0);
        GL::listen_opengl_errors();

    }

    template <typename... Args> void reload_impl(Args&&... args)
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
    }

    template <typename... Args> void destroy_impl(Args&&... args)
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
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        GL::listen_opengl_errors();

        glClear(GL_COLOR_BUFFER_BIT);
        GL::listen_opengl_errors();


        // Draw our first triangle
        glUseProgram(program_id_);
        GL::listen_opengl_errors();

        glBindVertexArray(VAO_);
        GL::listen_opengl_errors();

        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        GL::listen_opengl_errors();

        glBindVertexArray(0);
        GL::listen_opengl_errors();

    }

private:
    GLuint vertex_shader_;
    GLuint fragment_shader_;
    GLuint program_id_{};

    std::filesystem::path vertex_source_;
    std::filesystem::path fragment_source_;

    GLuint VBO_, VAO_, EBO_;
};

} // namespace SDL
