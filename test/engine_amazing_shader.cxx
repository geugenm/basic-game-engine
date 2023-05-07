#include <gtest/gtest.h>
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>

#include "opengl_functions.hxx"

// Read shader from file
std::string ReadShaderFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Initialize SDL and OpenGL context
void InitializeSDLAndOpenGL() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_Window* window = GL::create_window("", 500, 500);
    SDL_GLContext gl_context = GL::create_opengl_context(window);
}

// Clean up SDL and OpenGL context
void CleanupSDLAndOpenGL() {
    SDL_Quit();
}

// Compile shader
GLuint CompileShader(const std::string& shader_source, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);
    const char* source = shader_source.c_str();
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    return shader;
}

// Link shader program
GLuint LinkShaderProgram(GLuint vertex_shader, GLuint fragment_shader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    return program;
}

// Shader compilation test
TEST(ShaderTest, CompileAndLink) {
    InitializeSDLAndOpenGL();

    std::string shader_source = ReadShaderFromFile("amazing_shader.glsl");
    GLuint vertex_shader = CompileShader(shader_source, GL_VERTEX_SHADER);
    GLuint fragment_shader = CompileShader(shader_source, GL_FRAGMENT_SHADER);
    GLuint program = LinkShaderProgram(vertex_shader, fragment_shader);

    GLint compile_status, link_status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);

    // Check for compilation and linking errors
    EXPECT_EQ(compile_status, GL_TRUE);
    EXPECT_EQ(link_status, GL_TRUE);

    glDeleteProgram(program);
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);

    CleanupSDLAndOpenGL();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
