#include "engine.hxx"
#include "opengl_functions.hxx"
#include "render/declarations.hxx"
#include <gtest/gtest.h>

class MyEngine : public Engine::Instance
{
public:
    ~MyEngine() override = default;

    void initialize() override
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

        GLuint vertexShader =
            GL::load_shader(GL_VERTEX_SHADER, GL::read_file("vertex_shader.glsl"));
        GLuint fragmentShader =
            GL::load_shader(GL_FRAGMENT_SHADER, GL::read_file("fragment_shader.glsl"));

        program_id_ = glCreateProgram();
        glAttachShader(program_id_, vertexShader);
        glAttachShader(program_id_, fragmentShader);
        glLinkProgram(program_id_);

        GLfloat vertices[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f};

        glGenVertexArrays(1, &VAO_);
        glGenBuffers(1, &VBO_);
        glBindVertexArray(VAO_);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    void render() override
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program_id_);
        glBindVertexArray(VAO_);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        SDL_GL_SwapWindow(window_);
    }

    void destroy() override
    {
        glDeleteVertexArrays(1, &VAO_);
        glDeleteBuffers(1, &VBO_);
        glDeleteProgram(program_id_);

        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

private:
    SDL_Window* window_    = nullptr;
    SDL_GLContext context_ = nullptr;
    GLuint program_id_     = 0;

    GLuint VBO_, VAO_;

    Engine::Vertex3D<float>* alpha_ = new Engine::Vertex3D<float>(0.1f, 1.2f, 1.5f);
    Engine::Vertex3D<float>* beta_  = new Engine::Vertex3D<float>(54.1f, 3.2f, 7.1f);
    Engine::Vertex3D<float>* gamma_ = new Engine::Vertex3D<float>(125.1f, 275.2f, 2.6f);
};

Engine::Instance* Engine::Instance::create_instance()
{
    return new MyEngine();
}

TEST(TriangleTest, BasicInterpolation)
{
    Engine::Instance::instance().initialize();
    while (true)
    {
        Engine::Instance::instance().render();
    }
    Engine::Instance::instance().destroy();
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}