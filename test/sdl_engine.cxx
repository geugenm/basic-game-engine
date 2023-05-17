#include <sdl_engine.hxx>

#include <complex>
#include <gtest/gtest.h>

SDL::Vertex blend_vertex(const SDL::Vertex& vl,
                        const SDL::Vertex& vr,
                        const float       a)
{
    SDL::Vertex r;
    r.x = (1.0f - a) * vl.x + a * vr.x;
    r.y = (1.0f - a) * vl.y + a * vr.y;
    return r;
}

SDL::Triangle blend(const SDL::Triangle& tl,
                   const SDL::Triangle& tr,
                   const float         a)
{
    SDL::Triangle r;
    r.v[0] = blend_vertex(tl.v[0], tr.v[0], a);
    r.v[1] = blend_vertex(tl.v[1], tr.v[1], a);
    r.v[2] = blend_vertex(tl.v[2], tr.v[2], a);
    return r;
}


TEST(SDLEngineTest, Init)
{
    const char* window_title        = "TestSDLEngine";
    constexpr int32_t window_height = 1000;
    constexpr int32_t window_width  = 1000;
    SDL::Engine::Instance::instance().initialize(window_title, window_height, window_width);

    SDL::OpenGLShader shader;
    shader.initialize_impl("shaders/simple_vertex.glsl",
                           "shaders/simple_fragment.glsl");


    SDL::Triangle tr1q;
    tr1q.v[0] = SDL::Vertex{1.0, 1.0};
    tr1q.v[1] = SDL::Vertex{0.0, 0.1};
    tr1q.v[2] = SDL::Vertex{0.3, 0.5};

    SDL::Triangle tr2q;
    tr2q.v[0] = SDL::Vertex{1.0, 1.0};
    tr2q.v[1] = SDL::Vertex{0.6, 0.3};
    tr2q.v[2] = SDL::Vertex{0.2, 0.5};

    SDL::Triangle tr1t;
    tr1t.v[0] = SDL::Vertex{1.0, 1.0};
    tr1t.v[1] = SDL::Vertex{0.6, 0.3};
    tr1t.v[2] = SDL::Vertex{0.2, 0.5};

    SDL::Triangle tr2t;
    tr2t.v[0] = SDL::Vertex{1.0, 1.0};
    tr2t.v[1] = SDL::Vertex{0.6, 0.3};
    tr2t.v[2] = SDL::Vertex{0.2, 0.5};

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

        const float time = static_cast<float>(SDL_GetTicks()) / 1000.0f;

        float alpha = (std::sin(time) * 0.5f) + 0.5f;

        SDL::Triangle tr1 = blend(tr1q, tr1t, alpha);
        SDL::Triangle tr2 = blend(tr2q, tr2t, alpha);

        shader.render();

        SDL::Engine::Instance::instance().render();
    }

cleanup:
    SDL::Engine::Instance::instance().destroy();
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}