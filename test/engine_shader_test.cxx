#include "engine.hxx"
#include "opengl_functions.hxx"
#include "render/declarations.hxx"
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

        window_ = GL::create_window("Test", 500, 500);
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
        
        init_shaders();
        init_buffers();
    }

    template <typename... Args> void render_impl(GLfloat * vertices, Args&&... args)
    {
        GLfloat vertices1[] = {
            static_cast<float>(rand() % 1000) / 1000 - 0.5f,
            static_cast<float>(rand() % 1000) / 1000 - 0.5f,
            static_cast<float>(rand() % 1000) / 1000 - 0.5f,
            static_cast<float>(rand() % 1000) / 1000 - 0.5f,
            static_cast<float>(rand() % 1000) / 1000 - 0.5f,
            static_cast<float>(rand() % 1000) / 1000 - 0.5f,
        };

        // Update the vertex buffer with the new vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        GL::listen_opengl_errors();
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_DYNAMIC_DRAW);
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

    GLuint VBO_, VAO_;

    void init_shaders()
    {
        GLuint vertexShader =
            GL::load_shader(GL_VERTEX_SHADER, GL::read_file("vertex_shader.glsl"));
        GLuint fragmentShader =
            GL::load_shader(GL_FRAGMENT_SHADER, GL::read_file("fragment_shader.glsl"));

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
        GLfloat vertices[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f};

        glGenVertexArrays(1, &VAO_);
        GL::listen_opengl_errors();

        glGenBuffers(1, &VBO_);
        GL::listen_opengl_errors();

        glBindVertexArray(VAO_);
        GL::listen_opengl_errors();


        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        GL::listen_opengl_errors();

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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

        GLfloat vertices[] = {
            static_cast<float>(rand() % 1000) / 1000 - 0.5f,
            static_cast<float>(rand() % 1000) / 1000 - 0.5f,
            static_cast<float>(rand() % 1000) / 1000 - 0.5f,
            static_cast<float>(rand() % 1000) / 1000 - 0.5f,
            static_cast<float>(rand() % 1000) / 1000 - 0.5f,
            static_cast<float>(rand() % 1000) / 1000 - 0.5f,
        };

        MyEngine::Instance::instance().render(vertices);
    }

cleanup:
    MyEngine::Instance::instance().destroy();
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
