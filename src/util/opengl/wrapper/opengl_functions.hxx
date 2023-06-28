#pragma once

#include "opengl_settings.hxx"

#include <filesystem>
#include <string>
#include <vector>

#include <fstream>

namespace opengl_subsdk
{

bool is_opengl_version_supported();

std::string get_file_content(const std::filesystem::path &file_path);

GLuint get_new_compiled_shader(GLenum shader_type,
                               const GLchar *shader_content);

GLuint get_compiled_shader_from_file(GLenum shader_type,
                                     const std::filesystem::path &shader_path);

GLuint get_new_program();

void link_shader_program(GLuint program);

} // namespace opengl_subsdk
