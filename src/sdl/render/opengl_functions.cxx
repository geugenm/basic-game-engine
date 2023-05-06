#include "opengl_functions.hxx"

bool GL::init_sdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}


SDL_Window* GL::create_window(const char* window_title, const int& height, const int& width)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, kOpenGLMajorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, kOpenGLMinorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    constexpr Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

    SDL_Window* window = SDL_CreateWindow(window_title, height, width, flags);
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
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    if (major < kOpenGLMajorVersion ||
        (major == kOpenGLMajorVersion && minor < kOpenGLMinorVersion))
    {
        std::cerr << "Unsupported OpenGL version" << std::endl;
        return false;
    }

    return true;
}


std::string GL::read_shader_file(const std::filesystem::path& filePath)
{
    if (!std::filesystem::exists(filePath))
    {
        throw std::runtime_error("File not found: " + filePath.string());
    }

    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open())
    {
        throw std::runtime_error("Failed to open shader file: " + filePath.string());
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    return shaderStream.str();
}
