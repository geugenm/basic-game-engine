#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <stdexcept>

#include <SDL3/SDL.h>
#include <glad/glad.h>

namespace GL
{
constexpr uint16_t kOpenGLMajorVersion = 3;
constexpr uint16_t kOpenGLMinorVersion = 2;

bool init_sdl();

SDL_Window* create_window(const char* window_title, const int& height,
                                               const int& width);

SDL_GLContext create_opengl_context(SDL_Window* window);

bool load_opengl_functions();

void load_gl_func(const char* func_name);

bool is_opengl_version_supported();

std::string read_shader_file(const std::filesystem::path& filePath);

} // namespace GL
