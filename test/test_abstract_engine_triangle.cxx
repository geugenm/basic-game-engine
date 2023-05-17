#include "../src/sdl/opengl_functions.hxx"
#include "abstract_engine.hxx"
#include <gtest/gtest.h>

class MyEngine : public Engine::Instance<MyEngine>
{
public:
    ~MyEngine() override = default;

    template <typename... Args> void initialize_impl(Args&&... args)
    {
        if (!GL::init_sdl())
        {
            FAIL();
        }

        window_ = GL::create_window("Test", 1000, 1000);
        if (!window_)
        {
            SDL_Quit();
            FAIL();
        }

        context_ = GL::create_opengl_context(window_);
        if (!context_)
        {
            SDL_DestroyWindow(window_);
            SDL_Quit();
            FAIL();
        }

        if (!GL::load_opengl_functions() || !GL::is_opengl_version_supported())
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

        bool vertex_shader_changed =
            GL::has_shader_file_changed(k_vertex_shader_path_.data(), vertex_shader_last_modified);
        bool fragment_shader_changed =
            GL::has_shader_file_changed(k_fragment_shader_path_.data(), fragment_shader_last_modified);

        if (vertex_shader_changed || fragment_shader_changed)
        {
            glDeleteProgram(program_id_);
            GL::listen_opengl_errors();

            compile_shaders();
        }
    }

    template <typename... Args>
    void render_impl(const GLfloat vertices[], long vertices_size, Args&&... args)
    {
        shader_change_daemon();

        const float time = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        GLint timeLoc = glGetUniformLocation(program_id_, "time");
        glUniform1f(timeLoc, time);


        // Update the vertex buffer with the new vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        GL::listen_opengl_errors();
        glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);
        GL::listen_opengl_errors();

        // Re-bind the VAO after updating the vertex buffer
        glBindVertexArray(VAO_);
        GL::listen_opengl_errors();

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)nullptr);
        GL::listen_opengl_errors();

        glEnableVertexAttribArray(0);
        GL::listen_opengl_errors();

        glBindVertexArray(0);
        GL::listen_opengl_errors();

        // Clear the screen and draw the new triangle
        glClear(GL_COLOR_BUFFER_BIT);
        GL::listen_opengl_errors();

        glUseProgram(program_id_);
        GL::listen_opengl_errors();

        glBindVertexArray(VAO_);
        GL::listen_opengl_errors();

        glDrawArrays(GL_TRIANGLES, 0, 3);
        GL::listen_opengl_errors();

        glBindVertexArray(0);
        GL::listen_opengl_errors();

        SDL_GL_SwapWindow(window_);
        GL::listen_opengl_errors();
    }

    template <typename... Args> void destroy_impl(Args&&... args)
    {
        glDeleteVertexArrays(1, &VAO_);
        GL::listen_opengl_errors();

        glDeleteBuffers(1, &VBO_);
        GL::listen_opengl_errors();

        glDeleteProgram(program_id_);
        GL::listen_opengl_errors();

        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

private:
    SDL_Window* window_    = nullptr;
    SDL_GLContext context_ = nullptr;
    GLuint program_id_     = 0;

    static constexpr std::string_view k_vertex_shader_path_     = "shaders/triangle_vertex.glsl";
    static constexpr std::string_view k_fragment_shader_path_   = "shaders/triangle_fragment.glsl";

    GLuint VBO_, VAO_;

    void compile_shaders()
    {
        GLuint vertexShader =
            GL::load_shader(GL_VERTEX_SHADER, GL::get_file_content(k_vertex_shader_path_.data()));
        GLuint fragmentShader =
            GL::load_shader(GL_FRAGMENT_SHADER, GL::get_file_content(k_fragment_shader_path_.data()));

        program_id_ = glCreateProgram();
        GL::listen_opengl_errors();

        glAttachShader(program_id_, vertexShader);
        GL::listen_opengl_errors();

        glAttachShader(program_id_, fragmentShader);
        GL::listen_opengl_errors();

        glLinkProgram(program_id_);
        GL::listen_opengl_errors();
    }

    void init_buffers()
    {
        glGenVertexArrays(1, &VAO_);
        GL::listen_opengl_errors();

        glGenBuffers(1, &VBO_);
        GL::listen_opengl_errors();

        glBindVertexArray(VAO_);
        GL::listen_opengl_errors();

        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        GL::listen_opengl_errors();

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)nullptr);
        GL::listen_opengl_errors();

        glEnableVertexAttribArray(0);
        GL::listen_opengl_errors();

        glBindVertexArray(0);
        GL::listen_opengl_errors();
    }
};

template <> Engine::Instance<MyEngine>* MyEngine::Instance::create_instance()
{
    return new MyEngine();
}

TEST(TriangleTest, BasicInterpolation)
{
    MyEngine::Instance::instance().initialize();

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

        auto vertices = GL::parse_vertices_from_shader("../../test/shaders/triangle_vertex.glsl");

        MyEngine::Instance::instance().render(vertices.data(), vertices.size());
    }

cleanup:
    MyEngine::Instance::instance().destroy();
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}