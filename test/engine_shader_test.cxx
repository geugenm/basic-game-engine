#include "engine.hxx"
#include "opengl_functions.hxx"
#include "shader_vertex.hxx"
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

        GLuint vertex_shader                    = glCreateShader(GL_VERTEX_SHADER);
        const std::string vertex_shader_content = GL::read_shader_file("vertex_shader.glsl");
        const char* source                      = vertex_shader_content.data();

        glShaderSource(vertex_shader, 1, &source, nullptr);
        GL::listen_opengl_errors();

        glCompileShader(vertex_shader);
        GL::listen_opengl_errors();

        GLint vertex_shader_compilation_status = 0;
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_shader_compilation_status);
        GL::listen_opengl_errors();

        if (vertex_shader_compilation_status == 0)
        {
            GLint info_len = 0;
            glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &info_len);
            GL::listen_opengl_errors();
            std::vector<char> info_chars(static_cast<size_t>(info_len));
            glGetShaderInfoLog(vertex_shader, info_len, nullptr, info_chars.data());
            GL::listen_opengl_errors();
            glDeleteShader(vertex_shader);
            GL::listen_opengl_errors();

            std::string shader_type_name = "vertex";
            std::cerr << "Error compiling shader(vertex)\n"
                      << vertex_shader_content << "\n"
                      << info_chars.data();
            FAIL();
        }

        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        GL::listen_opengl_errors();
        const std::string fragment_shader_src = GL::read_shader_file("fragment_shader.glsl");
        source                                = fragment_shader_src.data();

        glShaderSource(fragment_shader, 1, &source, nullptr);
        GL::listen_opengl_errors();

        glCompileShader(fragment_shader);
        GL::listen_opengl_errors();

        GLint fragment_shader_compilation_status = 0;
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_shader_compilation_status);
        GL::listen_opengl_errors();

        if (fragment_shader_compilation_status == 0)
        {
            GLint info_len = 0;
            glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_len);
            GL::listen_opengl_errors();

            std::vector<char> info_chars(static_cast<size_t>(info_len));
            glGetShaderInfoLog(fragment_shader, info_len, nullptr, info_chars.data());
            GL::listen_opengl_errors();

            glDeleteShader(fragment_shader);
            GL::listen_opengl_errors();

            std::cerr << "Error compiling shader(fragment)\n"
                      << fragment_shader_src << "\n"
                      << info_chars.data();
            FAIL();
        }

        // now create program and attach vertex and fragment shaders

        program_id_ = glCreateProgram();
        GL::listen_opengl_errors();
        if (0 == program_id_)
        {
            std::cerr << "failed to create gl program";
            FAIL();
        }

        glAttachShader(program_id_, vertex_shader);
        GL::listen_opengl_errors();
        glAttachShader(program_id_, fragment_shader);
        GL::listen_opengl_errors();

        // bind attribute location
        glBindAttribLocation(program_id_, 0, "a_position");
        GL::listen_opengl_errors();

        // link program after binding attribute locations
        glLinkProgram(program_id_);
        GL::listen_opengl_errors();

        // Check the link status
        GLint linked_status = 0;
        glGetProgramiv(program_id_, GL_LINK_STATUS, &linked_status);
        GL::listen_opengl_errors();

        if (linked_status == 0)
        {
            GLint infoLen = 0;
            glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &infoLen);
            GL::listen_opengl_errors();

            std::vector<char> infoLog(static_cast<size_t>(infoLen));
            glGetProgramInfoLog(program_id_, infoLen, nullptr, infoLog.data());
            GL::listen_opengl_errors();

            std::cerr << "Error linking program:\n" << infoLog.data();
            glDeleteProgram(program_id_);
            GL::listen_opengl_errors();

            FAIL();
        }

        // turn on rendering with just created shader program
        glUseProgram(program_id_);
        GL::listen_opengl_errors();

        glEnable(GL_DEPTH_TEST);
    }

    void render() override
    {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);


        std::array<Engine::ShaderVertex, 3> triangle = {*alpha_, *beta_, *gamma_};

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Engine::ShaderVertex) * triangle.size(), triangle.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::ShaderVertex), (void*)0);
        GL::listen_opengl_errors();
        glEnableVertexAttribArray(0);
        GL::listen_opengl_errors();
        glValidateProgram(program_id_);
        GL::listen_opengl_errors();
        // Check the validate status
        GLint validate_status = 0;
        glGetProgramiv(program_id_, GL_VALIDATE_STATUS, &validate_status);
        GL::listen_opengl_errors();
        if (validate_status == GL_FALSE)
        {
            GLint infoLen = 0;
            glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &infoLen);
            GL::listen_opengl_errors();
            std::vector<char> infoLog(static_cast<size_t>(infoLen));
            glGetProgramInfoLog(program_id_, infoLen, nullptr, infoLog.data());
            GL::listen_opengl_errors();
            std::cerr << "Error linking program:\n" << infoLog.data();
            throw std::runtime_error("error");
        }
        glDrawArrays(GL_TRIANGLES, 0, 3);
        GL::listen_opengl_errors();

        glDeleteVertexArrays(1, &vao); // Clean up the VAO after rendering
        glDeleteBuffers(1, &vbo); // Clean up the VBO after rendering

        swap_buffers();
    }

    void swap_buffers()
    {
        SDL_GL_SwapWindow(window_);

        glClearColor(0.3f, 0.3f, 1.0f, 0.0f);
        GL::listen_opengl_errors();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GL::listen_opengl_errors();
    }


    void destroy() override
    {
        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

private:
    SDL_Window* window_    = nullptr;
    SDL_GLContext context_ = nullptr;
    GLuint program_id_     = 0;

    Engine::ShaderVertex * alpha_ = new Engine::ShaderVertex(0.1, 1.2);
    Engine::ShaderVertex * beta_ = new Engine::ShaderVertex(54.1, 3.2);
    Engine::ShaderVertex * gamma_  = new Engine::ShaderVertex(125.1, 275.2);
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
