#include "engine.hxx"
#include "opengl_functions.hxx"
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
    }

    void render() override
    {
        SDL_GL_SwapWindow(window_);
    }

    void destroy() override
    {
        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

private:
    SDL_Window* window_;

    SDL_GLContext context_;
};

Engine::Instance* Engine::Instance::create_instance()
{
    return new MyEngine();
}

TEST(TriangleTest, BasicInterpolation)
{
    Engine::Instance::instance().initialize();
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
