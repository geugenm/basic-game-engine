#pragma once

#include <glad/glad.h>
#include <memory>
#include <render/shader.hxx>
#include <stdexcept>

#include "opengl_functions.hxx"

namespace SDLEngine
{

class OpenGLShader : public AbstractEngine::IShader<OpenGLShader>
{
public:
    template <typename... Args>
    void initialize_impl(GLenum shaderType, const std::string& shaderSource, Args&&... args)
    {
        shader_id_ = glCreateShader(shaderType);
        GL::listen_opengl_errors();

        if (shader_id_ == 0)
        {
            throw std::runtime_error("Failed to create OpenGL shader");
        }

        const char* source = shaderSource.c_str();

        glShaderSource(shader_id_, 1, &source, nullptr);
        GL::listen_opengl_errors();
    }

    template <typename... Args> void compile_impl(Args&&... args)
    {
        glCompileShader(shader_id_);
        GL::listen_opengl_errors();

        GLint compileStatus;
        glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &compileStatus);
        GL::listen_opengl_errors();

        if (compileStatus == GL_FALSE)
        {
            GLint infoLogLength;
            glGetShaderiv(shader_id_, GL_INFO_LOG_LENGTH, &infoLogLength);
            GL::listen_opengl_errors();

            std::unique_ptr<char[]> infoLog(new char[static_cast<unsigned long>(infoLogLength)]);
            glGetShaderInfoLog(shader_id_, infoLogLength, nullptr, infoLog.get());
            GL::listen_opengl_errors();

            throw std::runtime_error("Shader compilation failed: " + std::string(infoLog.get()));
        }
    }

    template <typename... Args> void destroy_impl(Args&&... args)
    {
        if (shader_id_ != 0)
        {
            glDeleteShader(shader_id_);
            GL::listen_opengl_errors();

            shader_id_ = 0;
        }
    }

    template <typename... Args> void set_uniform_impl(Args&&... args)
    {

    }

    [[nodiscard]] GLuint get_shader_id() const
    {
        return shader_id_;
    }

protected:
    void set_shader_id(GLuint shaderId)
    {
        shader_id_ = shaderId;
    }

    GLuint & access_shader_id() {
        return shader_id_;
    }

private:
    GLuint shader_id_{0};
};




class CustomOpenGLShader : public OpenGLShader
{
public:
    template <typename... Args>
    void set_uniform_impl(const std::string& name, Args&&... args)
    {

    }
};


} // namespace SDLEngine
