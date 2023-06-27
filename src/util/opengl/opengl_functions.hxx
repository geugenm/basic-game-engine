#pragma once

#include "glad/glad.h"

#include <filesystem>
#include <string>
#include <vector>

#include <fstream>

#ifdef __ANDROID__

#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES3/gl3.h>

#endif // __ANDROID__

#ifndef OPENGL_MAJOR_VERSION
#define OPENGL_MAJOR_VERSION 3
#endif

#ifndef OPENGL_MINOR_VERSION
#define OPENGL_MINOR_VERSION 2
#endif

#ifndef OPENGL_INFO_LOG_SIZE
#define OPENGL_INFO_LOG_SIZE 512
#endif

namespace opengl_subsdk
{

bool is_opengl_version_supported();

std::string glenum_to_string(GLenum value);

static void GLAPIENTRY
opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                      [[maybe_unused]] GLsizei length, const GLchar *message,
                      [[maybe_unused]] const void *userParam);

void enable_debug_mode();

void disable_debug_mode();

bool file_has_changed(const std::string &file_path,
                      std::time_t &last_modified_time);

std::string get_file_content(const std::filesystem::path &file_path);

std::vector<GLfloat>
get_vertices_from_glsl_file(const std::string &shader_path);

GLuint get_new_compiled_shader(GLenum shader_type,
                               const GLchar *shader_content);

GLuint get_compiled_shader_from_file(GLenum shader_type,
                                     const std::filesystem::path &shader_path);

GLuint get_new_program();

void attach_shader(GLuint program, GLuint shader);

void link_shader_program(GLuint program);

void delete_shader(GLuint shader);

} // namespace opengl_subsdk
