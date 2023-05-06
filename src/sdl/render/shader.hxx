#include <glad/glad.h>
#include <iostream>
#include <string>

namespace Engine
{
class Shader
{
public:
    Shader(const std::string& vertex_path, const std::string& fragment_path);

    void use() const;
    template <typename T>
        requires std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, bool>
    void set_uniform(const std::string& name, T value) const;

private:
    unsigned int ID;
};
} // namespace Engine

namespace Engine
{
Shader::Shader(const std::string& vertex_path, const std::string& fragment_path)
{
    std::string vertexCode   = GL::read_shader_file(vertex_path);
    std::string fragmentCode = GL::read_shader_file(fragment_path);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, info_log);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, info_log);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, nullptr, info_log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const
{
    glUseProgram(ID);
}

template <typename T>
    requires std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, bool>
void Shader::set_uniform(const std::string& name, T value) const
{
    if constexpr (std::is_same_v<T, int>)
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    else if constexpr (std::is_same_v<T, float>)
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    else if constexpr (std::is_same_v<T, bool>)
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}
} // namespace Engine
