#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "SDL3/SDL.h"
#include "glad/glad.h"

namespace OpenGLWrapper
{

constexpr uint16_t k_opengl_major_version = 3;
constexpr uint16_t k_opengl_minor_version = 2;
constexpr uint32_t k_info_log_size        = 512;

bool init_sdl();

SDL_Window *get_new_sdl_window(const char *window_title, const int &height,
                               const int &width);

SDL_GLContext get_new_context(SDL_Window *window);

bool load_opengl_functions();

bool is_opengl_version_supported();

bool init_opengl();

void GLAPIENTRY opengl_debug_callback(GLenum source, GLenum type, GLuint id,
                                      GLenum severity, GLsizei length,
                                      const GLchar *message,
                                      const void *userParam);

void enable_debug_mode();

void disable_debug_mode();

GLuint get_shader_from_file(GLenum type, const std::string &source);

bool file_has_changed(const std::string &file_path,
                      std::time_t &last_modified_time);

std::vector<GLfloat>
get_vertices_from_glsl_file(const std::string &shader_path);

GLuint get_new_compiled_shader(GLenum shader_type,
                               const GLchar *shader_content);

GLuint get_new_program();

void attach_shader(GLuint program, GLuint shader);

void link_shader_program(GLuint program);

void delete_shader(GLuint shader);

void generate_vertex_array(GLsizei generated_names_amount,
                           GLuint *buffer_array);

void generate_buffer_object_name(GLsizei generated_names_amount,
                                 GLuint *buffer_array);

} // namespace OpenGLWrapper
