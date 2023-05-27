#include "opengl_functions.hxx"

#include <fstream>
#include <iostream>

#include <regex>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>

#include "glad/glad.h"

bool OpenGLWrapper::init_sdl()
{
    constexpr Uint32 flags = SDL_INIT_VIDEO;
    if (SDL_Init(flags) != 0)
    {
        return false;
    }
    return true;
}

SDL_Window *OpenGLWrapper::get_new_sdl_window(const char *window_title,
                                              const int &width,
                                              const int &height)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, k_opengl_major_version);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, k_opengl_minor_version);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    constexpr Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

    SDL_Window *window = SDL_CreateWindow(window_title, width, height, flags);
    if (!window)
    {
        std::cerr << "Failed to create SDL window: " << SDL_GetError()
                  << std::endl;
    }

    return window;
}

SDL_GLContext OpenGLWrapper::get_new_context(SDL_Window *window)
{
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError()
                  << std::endl;
    }

    return context;
}

bool OpenGLWrapper::load_opengl_functions()
{
    if (!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to load OpenGL functions with glad: "
                  << glGetError() << std::endl;
        return false;
    }
    return true;
}

bool OpenGLWrapper::is_opengl_version_supported()
{
    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);

    glGetIntegerv(GL_MINOR_VERSION, &minor);

    if (major < k_opengl_major_version ||
        (major == k_opengl_major_version && minor < k_opengl_minor_version))
    {
        return false;
    }

    return true;
}

bool OpenGLWrapper::init_opengl()
{
    if (!load_opengl_functions()) {
        std::cerr << "Failed to load OpenGL functions.";
        return false;
    }

    if (!is_opengl_version_supported()) {
        std::cerr << "OpenGL version is not supported.";
        return false;
    }

    return true;
}

void OpenGLWrapper::opengl_debug_callback(GLenum source, GLenum type, GLuint id,
                                          GLenum severity, GLsizei length,
                                          const GLchar *message,
                                          const void *userParam)
{
    std::cerr << "OpenGL Error:" << std::endl;
    std::cerr << "  Source: " << source << std::endl;
    std::cerr << "  Type: " << type << std::endl;
    std::cerr << "  ID: " << id << std::endl;
    std::cerr << "  Severity: " << severity << std::endl;
    std::cerr << "  Message: " << message << std::endl;
    std::cerr << std::endl;
}

void OpenGLWrapper::enable_debug_mode()
{
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(opengl_debug_callback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                              nullptr, GL_TRUE);
    }
}

void OpenGLWrapper::disable_debug_mode()
{
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glDisable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(nullptr, nullptr);
    }
}

GLuint OpenGLWrapper::get_shader_from_file(GLenum type,
                                           const std::string &source)
{
    GLuint shader = glCreateShader(type);

    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);

    glCompileShader(shader);

    return shader;
}

bool OpenGLWrapper::file_has_changed(const std::string &file_path,
                                     std::time_t &last_modified_time)
{
    struct stat file_stat
    {
    };
    if (stat(file_path.c_str(), &file_stat) != 0)
    {
        return false;
    }

    if (file_stat.st_mtime > last_modified_time)
    {
        last_modified_time = file_stat.st_mtime;
        return true;
    }

    return false;
}

std::vector<GLfloat>
OpenGLWrapper::get_vertices_from_glsl_file(const std::string &shader_path)
{
    std::ifstream file(shader_path);
    std::stringstream buf;
    buf << file.rdbuf();
    std::string shader_src = buf.str();

    std::regex vertex_regex(
        R"(\s*const\s+vec2\s+triangle_vertices\[\d\]\s*=\s*vec2\[\]\s*\()");

    std::smatch vertex_match;
    std::vector<GLfloat> vertices;

    if (std::regex_search(shader_src, vertex_match, vertex_regex))
    {
        std::string vertex_str = vertex_match.suffix().str();
        std::regex value_regex(R"(-?\d+(\.\d+)?f)");

        std::smatch value_match;
        while (std::regex_search(vertex_str, value_match, value_regex))
        {
            vertices.push_back(std::stof(value_match.str()));
            vertex_str = value_match.suffix().str();
        }
    }

    return vertices;
}

GLuint OpenGLWrapper::get_compiled_shader(GLenum shader_type,
                                          const GLchar *shader_content)
{
    GLenum result_shader = glCreateShader(shader_type);

    glShaderSource(result_shader, 1, &shader_content, nullptr);

    glCompileShader(result_shader);

    GLint success;
    glGetShaderiv(result_shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLchar info_log[k_info_log_size];
        glGetShaderInfoLog(result_shader, k_info_log_size, nullptr, info_log);

        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
                  << info_log << std::endl;
        throw std::runtime_error("Failed to compile shader");
    }

    return result_shader;
}

void OpenGLWrapper::link_shader_program(GLuint program)
{
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        GLchar info_log[k_info_log_size];
        glGetProgramInfoLog(program, k_info_log_size, nullptr, info_log);

        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << info_log << std::endl;
        throw std::runtime_error("Failed to link shader");
    }
}

GLuint OpenGLWrapper::get_new_program()
{
    GLuint program = glCreateProgram();

    if (program == 0)
    {
        throw std::runtime_error("Failed to create OpenGL program");
    }

    return program;
}

void OpenGLWrapper::attach_shader(GLuint program, GLuint shader)
{
    glAttachShader(program, shader);
}

void OpenGLWrapper::delete_shader(GLuint shader)
{
    glDeleteShader(shader);
}

void OpenGLWrapper::generate_vertex_array(GLsizei generated_names_amount,
                                          GLuint *buffer_array)
{
    glGenVertexArrays(generated_names_amount, buffer_array);
}

void OpenGLWrapper::generate_buffer_object_name(GLsizei generated_names_amount,
                                                GLuint *buffer_array)
{
    glGenBuffers(generated_names_amount, buffer_array);
}
