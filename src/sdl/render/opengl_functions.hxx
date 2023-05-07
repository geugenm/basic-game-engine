#pragma once

#include <filesystem>
#include <string>


#include <SDL3/SDL.h>
#include <glad/glad.h>

namespace GL
{

constexpr uint16_t kOpenGLMajorVersion = 3;
constexpr uint16_t kOpenGLMinorVersion = 2;

bool init_sdl();

SDL_Window* create_window(const char* window_title, const int& height, const int& width);

SDL_GLContext create_opengl_context(SDL_Window* window);

bool load_opengl_functions();

void load_gl_func(const char* func_name);

bool is_opengl_version_supported();

std::string read_shader_file(const std::filesystem::path& filePath);

void GLAPIENTRY opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                      GLsizei length, const GLchar* message, const void* userParam);

void listen_opengl_errors();

GLuint load_shader(GLenum type, const std::string& source);

std::string read_file(const std::filesystem::path& file_path);

bool has_shader_file_changed(const std::string& file_path, std::time_t& last_modified_time);

} // namespace GL
