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
    void initialize_impl(GLenum shaderType, const std::string& shaderSource)
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

    void compile_impl()
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

    void destroy_impl()
    {
        if (shader_id_ != 0)
        {
            glDeleteShader(shader_id_);
            GL::listen_opengl_errors();

            shader_id_ = 0;
        }
    }

private:
    GLuint shader_id_{0};
};

} // namespace SDLEngine
