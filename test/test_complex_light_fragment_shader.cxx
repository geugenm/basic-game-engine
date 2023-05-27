#include "glad/glad.h"
#include "opengl_functions.hxx"
#include "render/uniform.hxx"

#include <fstream>

#include <SDL3/SDL.h>
#include <gtest/gtest.h>

constexpr int k_window_width  = 1040;
constexpr int k_window_height = 585;

void set_uniforms(const GLuint &shader_program,
                  const Uniform<float, float, float, float> &uniform)
{
    const auto [mouse_x, mouse_y, mouse_click_x, mouse_click_y] =
        uniform.values;

    const float time = static_cast<float>(SDL_GetTicks()) / 1000.0f;

    GLint resolutionUniformLocation =
        glGetUniformLocation(shader_program, "iResolution");
    glUniform2f(resolutionUniformLocation, k_window_width, k_window_height);

    GLint timeUniformLocation = glGetUniformLocation(shader_program, "iTime");
    glUniform1f(timeUniformLocation, time);

    GLint mouseUniformLocation = glGetUniformLocation(shader_program, "iMouse");
    glUniform4f(mouseUniformLocation, mouse_x, -mouse_y + k_window_height,
                mouse_click_x, mouse_click_y - k_window_height);
}

std::string get_file_content(const std::string &file_path)
{
    std::ifstream file(file_path, std::ios::in | std::ios::binary);
    if (!file)
    {
        std::cerr << "Error: Unable to open file: " << file_path << std::endl;
        return "";
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
    std::string content(buffer.begin(), buffer.end());
    file.close();

    return content;
}

TEST(ShaderTest, ShaderOutput)
{
    OpenGLWrapper::init_sdl();

    SDL_Window *window = OpenGLWrapper::get_new_sdl_window(
        "Test", k_window_width, k_window_height);
    SDL_GLContext context = OpenGLWrapper::get_new_context(window);

    OpenGLWrapper::init_opengl();
    OpenGLWrapper::enable_debug_mode();

    glViewport(0, 0, k_window_width, k_window_height);

    const std::string vertexShaderSource =
        get_file_content("shaders/light_figures_rgb_vertex.glsl");
    const char *vertex_shader = vertexShaderSource.data();

    const std::string fragmentShaderSource =
        get_file_content("shaders/light_figures_rgb_fragment.glsl");
    const char *fragment_shader = fragmentShaderSource.data();

    GLuint vertexShader =
        OpenGLWrapper::get_new_compiled_shader(GL_VERTEX_SHADER, vertex_shader);

    GLuint fragmentShader = OpenGLWrapper::get_new_compiled_shader(
        GL_FRAGMENT_SHADER, fragment_shader);

    GLuint shaderProgram = OpenGLWrapper::get_new_program();
    OpenGLWrapper::attach_shader(shaderProgram, vertexShader);
    OpenGLWrapper::attach_shader(shaderProgram, fragmentShader);

    OpenGLWrapper::link_shader_program(shaderProgram);

    OpenGLWrapper::delete_shader(vertexShader);
    OpenGLWrapper::delete_shader(fragmentShader);

    GLfloat vertices[] = {-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 1.0f, 0.0f,
                          1.0f,  1.0f,  1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 0.0f};

    GLuint indices[] = {0, 1, 2, 0, 2, 3};

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          nullptr);
    glEnableVertexAttribArray(0);

    // Texture Coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          (GLvoid *)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    bool running = true;
    SDL_Event event;

    float mouseClickX = 0.0f;
    float mouseClickY = 0.0f;

    while (running)
    {

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                mouseClickX = event.button.x;
                mouseClickY = event.button.y;
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
            {
                mouseClickX = 0.f;
                mouseClickY = 0.f;
            }
        }

        // Set uniforms
        glUseProgram(shaderProgram);

        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        set_uniforms(shaderProgram,
                     Uniform<float, float, float, float>(
                         mouseX, mouseY, mouseClickX, mouseClickY));

        // Render quad
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        SDL_GL_SwapWindow(window);
    }

    // Clean up
    glDeleteProgram(shaderProgram);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
