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

SDL_GLContext OpenGLWrapper::get_new_sdl_gl_context(SDL_Window *window)
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

void OpenGLWrapper::init_opengl()
{
    if (!load_opengl_functions())
    {
        throw std::runtime_error("Failed to load OpenGL functions.");
    }

    if (!is_opengl_version_supported())
    {
        throw std::runtime_error("OpenGL version is not supported.");
    }
}

std::string OpenGLWrapper::glenum_to_string(GLenum value)
{
    switch (value) {
        case GL_DEBUG_SOURCE_API: return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW_SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER_COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD_PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER: return "OTHER";
        case GL_DEBUG_TYPE_ERROR: return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: return "MARKER";
        case GL_DEBUG_TYPE_PUSH_GROUP: return "PUSH_GROUP";
        case GL_DEBUG_TYPE_POP_GROUP: return "POP_GROUP";
        case GL_DEBUG_TYPE_OTHER: return "OTHER";
        case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
        case GL_DEBUG_SEVERITY_LOW: return "LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
        default: return "UNKNOWN";
    }
}

void OpenGLWrapper::opengl_debug_callback(GLenum source, GLenum type, GLuint id,
                                          GLenum severity, GLsizei length,
                                          const GLchar *message,
                                          const void *userParam)
{
    std::ostringstream msg;
    msg << "OpenGL Error:" << std::endl;
    msg << "  Source: " << glenum_to_string(source) << std::endl;
    msg << "  Type: " << glenum_to_string(type) << std::endl;
    msg << "  ID: " << id << std::endl;
    msg << "  Severity: " << glenum_to_string(severity) << std::endl;
    msg << "  Message: " << message << std::endl;
    msg << std::endl;

    if (msg.bad()) {
        throw std::invalid_argument("Can't print the error message: bad ostringstream");
    }

    std::cerr << msg.str() << std::flush;
}

void OpenGLWrapper::enable_debug_mode()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(opengl_debug_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                          GL_TRUE);
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

GLuint OpenGLWrapper::get_new_compiled_shader(GLenum shader_type,
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

char *OpenGLWrapper::get_file_content(const std::string &file_path)
{
    std::ifstream input_file(file_path);

    if (!input_file.is_open())
    {
        throw std::invalid_argument("Unable to open file: " + file_path);
    }

    std::string content((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

    if (content.empty()) {
        std::cerr << "Trying to read empty file: " << file_path << std::endl;
        return nullptr;
    }

    const size_t content_size = content.size();
    char *result = static_cast<char *>(std::malloc(content_size + 1));
    std::copy(content.begin(), content.end(), result);
    result[content_size] = '\0';

    return result;
}

GLuint OpenGLWrapper::get_compiled_shader_from_file(GLenum shader_type,
                                                    const char *shader_path)
{
    GLchar *shader_content = OpenGLWrapper::get_file_content(shader_path);

    GLuint result =
        OpenGLWrapper::get_new_compiled_shader(shader_type, shader_content);

    return result;
}
