#include "opengl_functions.hxx"

#include <fstream>
#include <iostream>

#include <regex>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>

#include "glad/glad.h"

bool GL::init_sdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

SDL_Window* GL::create_window(const char* window_title, const int& width, const int& height)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, kOpenGLMajorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, kOpenGLMinorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    constexpr Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

    SDL_Window* window = SDL_CreateWindow(window_title, width, height, flags);
    if (!window)
    {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
    }

    return window;
}

SDL_GLContext GL::create_opengl_context(SDL_Window* window)
{
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
    }

    return context;
}

bool GL::load_opengl_functions()
{
    if (!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to load OpenGL functions with glad: " << glGetError() << std::endl;
        return false;
    }
    return true;
}

void GL::load_gl_func(const char* func_name)
{
    SDL_FunctionPointer gl_pointer = SDL_GL_GetProcAddress(func_name);
    if (nullptr == gl_pointer)
    {
        throw std::runtime_error(std::string("Can't load GL function ") + func_name);
    }
}

bool GL::is_opengl_version_supported()
{
    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    GL::listen_opengl_errors();

    glGetIntegerv(GL_MINOR_VERSION, &minor);
    GL::listen_opengl_errors();

    if (major < kOpenGLMajorVersion ||
        (major == kOpenGLMajorVersion && minor < kOpenGLMinorVersion))
    {
        std::cerr << "Unsupported OpenGL version" << std::endl;
        return false;
    }

    return true;
}

void GL::opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                               GLsizei length, const GLchar* message, const void* userParam)
{
    std::cerr << "OpenGL Error:" << std::endl;
    std::cerr << "  Source: " << source << std::endl;
    std::cerr << "  Type: " << type << std::endl;
    std::cerr << "  ID: " << id << std::endl;
    std::cerr << "  Severity: " << severity << std::endl;
    std::cerr << "  Message: " << message << std::endl;
    std::cerr << std::endl;
}

void GL::listen_opengl_errors()
{
    glEnable(GL_DEBUG_OUTPUT);

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(opengl_debug_callback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}

GLuint GL::load_shader(GLenum type, const std::string& source)
{
    GLuint shader = glCreateShader(type);
    GL::listen_opengl_errors();

    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    GL::listen_opengl_errors();

    glCompileShader(shader);
    GL::listen_opengl_errors();

    return shader;
}

std::string GL::get_file_content(const std::string& file_path)
{
    std::ifstream file(file_path, std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open " + std::filesystem::path(file_path).string());
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool GL::has_shader_file_changed(const std::string& file_path, time_t& last_modified_time)
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

std::vector<GLfloat> GL::parse_vertices_from_shader(const std::string& shader_path)
{
    std::ifstream file(shader_path);
    std::stringstream buf;
    buf << file.rdbuf();
    std::string shader_src = buf.str();

    std::regex vertex_regex(R"(\s*const\s+vec2\s+triangle_vertices\[\d\]\s*=\s*vec2\[\]\s*\()");

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

GLuint GL::link_shader_program(GLuint& shader)
{
    GLuint program_id = glCreateProgram();
    GL::listen_opengl_errors();

    if (0 == program_id)
    {
        std::cerr << "failed to create gl program";
        throw std::runtime_error("can't link shader");
    }

    glAttachShader(program_id, shader);
    GL::listen_opengl_errors();

    glLinkProgram(program_id);
    GL::listen_opengl_errors();

    GLint linked_status = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked_status);
    GL::listen_opengl_errors();

    if (linked_status == 0)
    {
        GLint infoLen = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &infoLen);
        GL::listen_opengl_errors();

        std::vector<char> infoLog(static_cast<size_t>(infoLen));
        glGetProgramInfoLog(program_id, infoLen, nullptr, infoLog.data());
        GL::listen_opengl_errors();

        std::cerr << "Error linking program:\n" << infoLog.data();
        glDeleteProgram(program_id);
        GL::listen_opengl_errors();

        return 0;
    }
    return program_id;
}

void GL::destroy_shader(GLuint program_id)
{
    glDeleteShader(program_id);
    GL::listen_opengl_errors();
}

void GL::use_shader(GLuint program_id)
{
    glUseProgram(program_id);
    GL::listen_opengl_errors();
}

int GL::get_uniform_location(const GLuint& program_id, const char* uniform_name)
{
    const int location = glGetUniformLocation(program_id, uniform_name);
    GL::listen_opengl_errors();

    constexpr int uniform_not_found = -1;

    if (location == uniform_not_found)
    {
        throw std::invalid_argument("Uniform " + std::string(uniform_name) + " was not found");
    }

    return location;
}

GLuint GL::compile_shader(const GLenum& shader_type, const char* shader_content)
{
    GLuint shader_id = glCreateShader(shader_type);
    GL::listen_opengl_errors();

    glShaderSource(shader_id, 1, &shader_content, nullptr);
    GL::listen_opengl_errors();

    glCompileShader(shader_id);
    GL::listen_opengl_errors();

    GLint compiled_status = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_status);
    GL::listen_opengl_errors();

    if (compiled_status == 0)
    {
        GLint info_len = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_len);
        GL::listen_opengl_errors();

        std::vector<char> info_chars(static_cast<size_t>(info_len));
        glGetShaderInfoLog(shader_id, info_len, nullptr, info_chars.data());
        GL::listen_opengl_errors();

        glDeleteShader(shader_id);
        GL::listen_opengl_errors();

        std::string shader_type_name = shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment";
        std::cerr << "Error compiling shader(vertex)\n" << info_chars.data();
        return 0;
    }
    return shader_id;
}
