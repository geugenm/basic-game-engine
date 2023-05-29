#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <SDL3/SDL.h>
#include <fstream>
#include <glad/glad.h>

namespace OpenGLWrapper
{

constexpr uint16_t k_opengl_major_version = 3;
constexpr uint16_t k_opengl_minor_version = 2;
constexpr uint32_t k_info_log_size        = 512;

void init_sdl();

SDL_Window *get_new_sdl_window(const char *window_title, const int &height,
                               const int &width);

SDL_GLContext get_new_sdl_gl_context(SDL_Window *window);

void load_opengl_functions();

bool is_opengl_version_supported();

void init_opengl();

std::string glenum_to_string(GLenum value);

static void GLAPIENTRY opengl_debug_callback(GLenum source, GLenum type,
                                             GLuint id, GLenum severity,
                                             GLsizei length,
                                             const GLchar *message,
                                             const void *userParam);

void enable_debug_mode();

void disable_debug_mode();

bool file_has_changed(const std::string &file_path,
                      std::time_t &last_modified_time);

char *get_file_content(const std::string &file_path);

std::vector<GLfloat>
get_vertices_from_glsl_file(const std::string &shader_path);

GLuint get_new_compiled_shader(GLenum shader_type,
                               const GLchar *shader_content);

GLuint get_compiled_shader_from_file(GLenum shader_type,
                                     const char *shader_path);

GLuint get_new_program();

void attach_shader(GLuint program, GLuint shader);

void link_shader_program(GLuint program);

void delete_shader(GLuint shader);

void generate_vertex_array(GLsizei generated_names_amount,
                           GLuint *buffer_array);

void generate_buffer_object_name(GLsizei generated_names_amount,
                                 GLuint *buffer_array);


void unbind_texture(GLenum texture_type);

} // namespace OpenGLWrapper
