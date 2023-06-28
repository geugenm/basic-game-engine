#include "opengl_functions.hxx"

#include <fstream>
#include <iostream>
#include <stdexcept>

bool opengl_subsdk::is_opengl_version_supported()
{
    int major;
    int minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);

    glGetIntegerv(GL_MINOR_VERSION, &minor);

    if (major < OPENGL_CURRENT_MAJOR_VERSION ||
        (major == OPENGL_CURRENT_MAJOR_VERSION &&
         minor < OPENGL_CURRENT_MINOR_VERSION))
    {
        return false;
    }

    return true;
}

GLuint opengl_subsdk::get_new_compiled_shader(GLenum shader_type,
                                              const GLchar *shader_content)
{
    if (shader_content == nullptr)
    {
        throw std::invalid_argument("Shader content is nullptr");
    }

    GLenum result_shader = glCreateShader(shader_type);

    glShaderSource(result_shader, 1, &shader_content, nullptr);

    glCompileShader(result_shader);

    GLint success;
    glGetShaderiv(result_shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLint logSize = 0;
        glGetShaderiv(result_shader, GL_INFO_LOG_LENGTH, &logSize);
        std::vector<GLchar> log(static_cast<unsigned long>(logSize));
        glGetShaderInfoLog(result_shader, logSize, nullptr, log.data());

        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
                  << log.data() << std::endl;
        throw std::domain_error("Failed to compile shader: " +
                                std::string(log.data()));
    }

    return result_shader;
}

void opengl_subsdk::link_shader_program(GLuint program)
{
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        std::string log;
        glGetProgramInfoLog(program, OPENGL_INFO_LOG_SIZE, nullptr, log.data());

        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << log << std::endl;
        throw std::domain_error("Failed to link shader" + log);
    }
}

GLuint opengl_subsdk::get_new_program()
{
    GLuint program = glCreateProgram();

    if (program == 0)
    {
        throw std::domain_error("Failed to create OpenGL program");
    }

    return program;
}

std::string
opengl_subsdk::get_file_content(const std::filesystem::path &file_path)
{
    if (!exists(file_path))
    {
        throw std::invalid_argument("File '" + file_path.string() +
                                    "' does not exist");
    }

    std::ifstream input_file(file_path);
    if (!input_file.is_open())
    {
        throw std::invalid_argument("Unable to open file: " +
                                    file_path.string());
    }

    std::string content((std::istreambuf_iterator<char>(input_file)),
                        std::istreambuf_iterator<char>());

    if (content.empty())
    {
        throw std::invalid_argument("File is empty");
    }

    return content;
}

GLuint opengl_subsdk::get_compiled_shader_from_file(
    GLenum shader_type, const std::filesystem::path &shader_path)
{
    if (!exists(shader_path))
    {
        throw std::invalid_argument("File '" + shader_path.string() +
                                    "' does not exist");
    }

    const std::string shader_content =
        opengl_subsdk::get_file_content(shader_path);

    const GLuint result = opengl_subsdk::get_new_compiled_shader(
        shader_type, shader_content.data());

    return result;
}
