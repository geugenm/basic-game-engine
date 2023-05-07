#include <glad/glad.h>
#include <gtest/gtest.h>
#include <sdl_engine.hxx>
#include <sdl_shader.hxx>

class MyEngine : public SDLEngine::Instance
{
public:


protected:
    void compile_shaders() override
    {
        vertex_shader_                                 = new SDLEngine::OpenGLVertexShader();
        const std::filesystem::path vertex_shader_path = "vertex_shader.glsl";

        if (vertex_shader_)
        {
            vertex_shader_->initialize_impl(vertex_shader_path);
            vertex_shader_->compile_impl();
        }

        fragment_shader_                                 = new SDLEngine::OpenGLFragmentShader();
        const std::filesystem::path fragment_shader_path = "fragment_shader.glsl";

        if (fragment_shader_)
        {
            fragment_shader_->initialize_impl(fragment_shader_path);
            fragment_shader_->compile_impl();
        }
    }

    void reload_shaders() override {
        if (vertex_shader_) {
            vertex_shader_->reload();
        }

        if (fragment_shader_) {
            fragment_shader_->reload();
        }
    }
};

TEST(TestMyEngine, TestGeneral) {
    MyEngine::instance().initialize("some_window", 1000, 1000);

    while (true) {
        MyEngine::instance().render();
    }

    MyEngine::instance().destroy();
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



