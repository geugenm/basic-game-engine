#include "abstract_engine.hxx"
#include "opengl_functions.hxx"
#include <gtest/gtest.h>

#include <fstream>

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

class MyEngine : public Engine::Instance<MyEngine>
{
public:
    ~MyEngine() override = default;

    void initialize_impl()
    {
        if (!OpenGLWrapper::init_sdl())
        {
            FAIL();
        }

        window_ = OpenGLWrapper::get_new_sdl_window("Test", 1000, 1000);
        if (!window_)
        {
            SDL_Quit();
            FAIL();
        }

        context_ = OpenGLWrapper::get_new_sdl_gl_context(window_);
        if (!context_)
        {
            SDL_DestroyWindow(window_);
            SDL_Quit();
            FAIL();
        }

        if (!OpenGLWrapper::load_opengl_functions() ||
            !OpenGLWrapper::is_opengl_version_supported())
        {
            SDL_GL_DeleteContext(context_);
            SDL_DestroyWindow(window_);
            SDL_Quit();
            FAIL();
        }

        compile_shaders();
        init_buffers();
    }

    void shader_change_daemon()
    {
        static std::time_t vertex_shader_last_modified   = 0;
        static std::time_t fragment_shader_last_modified = 0;

        bool vertex_shader_changed = OpenGLWrapper::file_has_changed(
            k_vertex_shader_path_.data(), vertex_shader_last_modified);
        bool fragment_shader_changed = OpenGLWrapper::file_has_changed(
            k_fragment_shader_path_.data(), fragment_shader_last_modified);

        if (vertex_shader_changed || fragment_shader_changed)
        {
            glDeleteProgram(program_id_);

            compile_shaders();
        }
    }

    void render_impl(const GLfloat vertices[], long vertices_size)
    {
        shader_change_daemon();

        const float time = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        GLint timeLoc    = glGetUniformLocation(program_id_, "time");
        glUniform1f(timeLoc, time);

        // Update the vertex buffer with the new vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);

        glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);

        // Re-bind the VAO after updating the vertex buffer
        glBindVertexArray(VAO_);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                              (GLvoid *)nullptr);

        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        // Clear the screen and draw the new triangle
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program_id_);

        glBindVertexArray(VAO_);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        SDL_GL_SwapWindow(window_);
    }

    void destroy_impl()
    {
        glDeleteVertexArrays(1, &VAO_);

        glDeleteBuffers(1, &VBO_);

        glDeleteProgram(program_id_);

        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

private:
    SDL_Window *window_    = nullptr;
    SDL_GLContext context_ = nullptr;
    GLuint program_id_     = 0;

    static constexpr std::string_view k_vertex_shader_path_ =
        "shaders/triangle_vertex.glsl";
    static constexpr std::string_view k_fragment_shader_path_ =
        "shaders/triangle_fragment.glsl";

    GLuint VBO_, VAO_;

    void compile_shaders()
    {
        GLuint vertexShader = OpenGLWrapper::get_compiled_shader(
            GL_VERTEX_SHADER, get_file_content(k_vertex_shader_path_.data()));
        GLuint fragmentShader = OpenGLWrapper::get_compiled_shader(
            GL_FRAGMENT_SHADER,
            get_file_content(k_fragment_shader_path_.data()));

        program_id_ = glCreateProgram();

        glAttachShader(program_id_, vertexShader);

        glAttachShader(program_id_, fragmentShader);

        glLinkProgram(program_id_);
    }

    void init_buffers()
    {
        glGenVertexArrays(1, &VAO_);

        glGenBuffers(1, &VBO_);

        glBindVertexArray(VAO_);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                              (GLvoid *)nullptr);

        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }
};

template <> Engine::Instance<MyEngine> *Engine::create_instance()
{
    return new MyEngine();
}

TEST(TriangleTest, LavaLampTriangle)
{
    Engine::Instance<MyEngine> *engine = Engine::create_instance<MyEngine>();
    engine->initialize();

    SDL_Event event;
    while (true)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                goto cleanup;
            }
        }

        auto vertices = OpenGLWrapper::get_vertices_from_glsl_file(
            "shaders/triangle_vertex.glsl");

        engine->render(vertices.data(), vertices.size());
    }

cleanup:
    engine->destroy();
}

auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
