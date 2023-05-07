#include <glad/glad.h>
#include <sdl_shader.hxx>
#include <gtest/gtest.h>

class OpenGLShaderTest : public ::testing::Test {
protected:
    SDLEngine::OpenGLShader shader;
};

TEST_F(OpenGLShaderTest, InitializationSuccess) {
    GLenum shaderType = GL_VERTEX_SHADER;
    std::string shaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    ASSERT_NO_THROW(shader.initialize_impl(shaderType, shaderSource));
}

TEST_F(OpenGLShaderTest, InitializationFailure) {
    GLenum shaderType = GL_INVALID_ENUM;
    std::string shaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    ASSERT_THROW(shader.initialize_impl(shaderType, shaderSource), std::runtime_error);
}

TEST_F(OpenGLShaderTest, CompilationSuccess) {
    GLenum shaderType = GL_VERTEX_SHADER;
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

TEST_F(OpenGLShaderTest, CompilationFailure) {
    GLenum shaderType = GL_VERTEX_SHADER;
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

TEST_F(OpenGLShaderTest, Destruction) {
    GLenum shaderType = GL_VERTEX_SHADER;
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

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
