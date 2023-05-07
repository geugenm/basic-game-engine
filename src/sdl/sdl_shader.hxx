#pragma once

#include <glad/glad.h>
#include <memory>
#include <render/shader.hxx>
#include <stdexcept>
#include <functional>

#include "opengl_functions.hxx"

namespace SDLEngine
{

class OpenGLShader : public AbstractEngine::IShader<OpenGLShader>
{
public:
    template <typename... Args>
    void initialize_impl(GLenum shader_type, const char* shader_content, Args&&... args)
    {
        type_ = shader_type;
        id_ = glCreateShader(shader_type);
        GL::listen_opengl_errors();

        if (id_ == 0)
        {
            throw std::runtime_error("Failed to create OpenGL shader");
        }

        const char* source = shader_content;

        glShaderSource(id_, 1, &source, nullptr);
        GL::listen_opengl_errors();
    }

    template <typename... Args>
    void initialize_impl(GLenum shader_type, const std::filesystem::path& shader_path,
                         Args&&... args)
    {
        source_ = shader_path;
        const std::string shader_content = GL::read_file(shader_path);
        initialize_impl(shader_type, shader_content.data());
    }

    template <typename... Args> void reload_impl(Args&&... args)
    {
        if (source_.empty())
        {
            throw std::invalid_argument("Can't reload shader, source was not defined. Shader was "
                                        "initialized via raw code.");
        }

        compile_impl();
    }

    template <typename... Args> void compile_impl(Args&&... args)
    {
        glCompileShader(id_);
        GL::listen_opengl_errors();

        GLint compileStatus;
        glGetShaderiv(id_, GL_COMPILE_STATUS, &compileStatus);
        GL::listen_opengl_errors();

        if (compileStatus == GL_FALSE)
        {
            GLint infoLogLength;
            glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength);
            GL::listen_opengl_errors();

            std::unique_ptr<char[]> infoLog(new char[static_cast<unsigned long>(infoLogLength)]);
            glGetShaderInfoLog(id_, infoLogLength, nullptr, infoLog.get());
            GL::listen_opengl_errors();

            throw std::runtime_error("Shader compilation failed: " + std::string(infoLog.get()));
        }
    }

    template <typename... Args> void destroy_impl(Args&&... args)
    {
        if (id_ != 0)
        {
            glDeleteShader(id_);
            GL::listen_opengl_errors();

            id_ = 0;
        }
    }

    template <typename... Args, typename T>
    void set_uniform_impl(const std::string& name, const T& value,
                          const std::function<void(GLint, const T&)>& uniform_setter,
                          Args&&... args)
    {
    }

    [[nodiscard]] GLuint get_shader_id() const
    {
        return id_;
    }

protected:
    void set_shader_id(GLuint shaderId)
    {
        id_ = shaderId;
    }

    GLuint& access_shader_id()
    {
        return id_;
    }

private:
    GLuint id_{0};
    GLenum type_;

    std::filesystem::path source_;
};

class OpenGLVertexShader : public OpenGLShader
{
public:
    template <typename... Args> void initialize_impl(const char* shader_content, Args&&... args)
    {
        OpenGLShader::initialize_impl(GL_VERTEX_SHADER, shader_content);
    }

    template <typename... Args>
    void initialize_impl(const std::filesystem::path& shader_path, Args&&... args)
    {
        OpenGLShader::initialize_impl(GL_VERTEX_SHADER, shader_path);
    }
};

class OpenGLFragmentShader : public OpenGLShader
{
public:
    template <typename... Args> void initialize_impl(const char* shader_content, Args&&... args)
    {
        OpenGLShader::initialize_impl(GL_FRAGMENT_SHADER, shader_content);
    }

    template <typename... Args>
    void initialize_impl(const std::filesystem::path& shader_path, Args&&... args)
    {
        OpenGLShader::initialize_impl(GL_FRAGMENT_SHADER, shader_path);
    }
};

} // namespace SDLEngine
