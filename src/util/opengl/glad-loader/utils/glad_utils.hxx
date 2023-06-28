#pragma once

#include <glad/glad.h>
#include <string>

namespace opengl_subsdk
{

std::string glenum_to_string(GLenum value);

static void GLAPIENTRY
opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                      [[maybe_unused]] GLsizei length, const GLchar *message,
                      [[maybe_unused]] const void *userParam);

void enable_debug_mode();

void disable_debug_mode();

} // namespace opengl_subsdk
