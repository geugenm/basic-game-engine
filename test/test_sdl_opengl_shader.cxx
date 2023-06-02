#include "opengl_functions.hxx"
#include <glad/glad.h>
#include <gtest/gtest.h>
#include <sdl_shader.hxx>

class OpenGLShaderTest : public ::testing::Test
{
public:
    OpenGLShaderTest()
    {
        if (!opengl_subsdk::init_sdl())
        {
            throw std::runtime_error("Failed to init sdl.");
        }

        window_ = opengl_subsdk::get_new_sdl_window("", 100, 200);

        if (!window_)
        {
            SDL_Quit();
            throw std::runtime_error("Failed to init window.");
        }

        context_ = opengl_subsdk::get_new_sdl_gl_context(window_);
        if (!context_)
        {
            SDL_DestroyWindow(window_);
            SDL_Quit();
            throw std::runtime_error("Failed to init create OpenGL context.");
        }

        if (!opengl_subsdk::load_opengl_functions() ||
            !opengl_subsdk::is_opengl_version_supported())
        {
            SDL_GL_DeleteContext(context_);
            SDL_DestroyWindow(window_);
            SDL_Quit();
            throw std::runtime_error("Failed to load opengl functions or "
                                     "opengl version is not supported.");
        }
    }

protected:
    SDL::OpenGLShader<SDL::SomeShader> shader;

    SDL_Window *window_ = nullptr;

    SDL_GLContext context_;
};

TEST_F(OpenGLShaderTest, InitializationSuccess)
{
    GLenum shaderType        = GL_VERTEX_SHADER;
    std::string shaderSource = R"(
        #version 320 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    ASSERT_NO_THROW(shader.initialize_impl(shaderType, shaderSource));
}

TEST_F(OpenGLShaderTest, InitializationFailure)
{
    GLenum shaderType        = GL_INVALID_ENUM;
    std::string shaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    ASSERT_THROW(shader.initialize_impl(shaderType, shaderSource),
                 std::runtime_error);
}

TEST_F(OpenGLShaderTest, CompilationSuccess)
{
    GLenum shaderType        = GL_VERTEX_SHADER;
    std::string shaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    shader.initialize_impl(shaderType, shaderSource);
    ASSERT_NO_THROW(shader.compile_impl());
}

TEST_F(OpenGLShaderTest, CompilationFailure)
{
    GLenum shaderType        = GL_VERTEX_SHADER;
    std::string shaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos); // Missing the last component
        }
    )";

    shader.initialize_impl(shaderType, shaderSource);
    ASSERT_THROW(shader.compile_impl(), std::runtime_error);
}

TEST_F(OpenGLShaderTest, Destruction)
{
    GLenum shaderType        = GL_VERTEX_SHADER;
    std::string shaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    shader.initialize_impl(shaderType, shaderSource);
    shader.compile_impl();
    ASSERT_NO_THROW(shader.destroy_impl());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
