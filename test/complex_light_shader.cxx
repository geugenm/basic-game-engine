#include "glad/glad.h"
#include "opengl_functions.hxx"
#include "render/uniform.hxx"

#include <SDL.h>
#include <gtest/gtest.h>

constexpr int k_window_width  = 1040;
constexpr int k_window_height = 585;

void set_uniforms(const GLuint& shader_program, const Uniform<float, float, float, float>& uniform)
{
    const auto [mouse_x, mouse_y, mouse_click_x, mouse_click_y] = uniform.values;

    const float time = static_cast<float>(SDL_GetTicks()) / 1000.0f;

    GLint resolutionUniformLocation = glGetUniformLocation(shader_program, "iResolution");
    glUniform2f(resolutionUniformLocation, k_window_width, k_window_height);

    GLint timeUniformLocation = glGetUniformLocation(shader_program, "iTime");
    glUniform1f(timeUniformLocation, time);

    GLint mouseUniformLocation = glGetUniformLocation(shader_program, "iMouse");
    glUniform4f(mouseUniformLocation, mouse_x, -mouse_y + k_window_height, mouse_click_x,
                mouse_click_y - k_window_height);
}

bool check_shader_compile_status(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
        return false;
    }
    return true;
}

bool check_program_link_status(GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program linking failed: " << infoLog << std::endl;
        return false;
    }
    return true;
}


TEST(ShaderTest, ShaderOutput)
{
    GL::init_sdl();
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_Window* window    = GL::create_window("Test", k_window_width, k_window_height);
    SDL_GLContext context = GL::create_opengl_context(window);

    GL::load_opengl_functions();
    GL::is_opengl_version_supported();

    glDebugMessageCallback(GL::opengl_debug_callback, nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glViewport(0, 0, k_window_width, k_window_height);
    GL::listen_opengl_errors();

    const std::string vertexShaderSource =
        GL::read_file("shaders/light_figures_rgb_vertex.glsl");
    const char* vertex_shader = vertexShaderSource.data();

    const std::string fragmentShaderSource =
        GL::read_file("shaders/light_figures_rgb_fragment.glsl");
    const char* fragment_shader = fragmentShaderSource.data();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader, nullptr);
    glCompileShader(vertexShader);
    if (!check_shader_compile_status(vertexShader)) {
        FAIL();
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader, nullptr);
    glCompileShader(fragmentShader);
    if (!check_shader_compile_status(fragmentShader)) {
        FAIL();
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    if (!check_program_link_status(shaderProgram)) {
        FAIL();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Texture Coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          (GLvoid*)(2 * sizeof(GLfloat)));
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
                     Uniform<float, float, float, float>(mouseX, mouseY, mouseClickX, mouseClickY));

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

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
