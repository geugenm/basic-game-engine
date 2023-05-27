#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "SDL3/SDL.h"
#include "glad/glad.h"

namespace GL
{

constexpr uint16_t k_opengl_major_version = 3;
constexpr uint16_t k_opengl_minor_version = 2;
constexpr uint32_t k_info_log_size        = 512;

bool init_sdl();

SDL_Window *create_window(const char *window_title, const int &height, const int &width);

SDL_GLContext create_opengl_context(SDL_Window *window);

bool load_opengl_functions();

void load_gl_func(const char *func_name);

bool is_opengl_version_supported();

void GLAPIENTRY opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                      const GLchar *message, const void *userParam);

void listen_opengl_errors();

GLuint load_shader(GLenum type, const std::string &source);

std::string get_file_content(const std::string &file_path);

bool has_shader_file_changed(const std::string &file_path, std::time_t &last_modified_time);

std::vector<GLfloat> parse_vertices_from_shader(const std::string &shader_path);

GLuint compile_shader(GLenum shader_type, const GLchar *shader_content);

GLuint create_program();

void attach_shader(GLuint program, GLuint shader);

void link_shader_program(GLuint program);

void delete_shader(GLuint shader);

void generate_vertex_array(GLsizei n, GLuint *arrays);

void generate_buffer_object_name(GLsizei n, GLuint *arrays);

} // namespace GL
