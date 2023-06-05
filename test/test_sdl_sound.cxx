#include "sdl_sound.hxx"
#include "opengl_functions.hxx"

#include <SDL3/SDL.h>
#include <cmath>
#include <gtest/gtest.h>

TEST(SDL_Audio, test_audio_mixer_class)
{
    auto mixer = new sdl_subsdk::audio_mixer("wav/machine_gun.WAV");
    mixer->initialize();

    auto sound2 = new sdl_subsdk::audio_mixer("wav/car_on.WAV");
    sound2->initialize();

    auto *engine =
        new sdl_subsdk::engine("OpenGL 3.0 SDL Sound Test", 1280, 720);

    engine->initialize();
    opengl_subsdk::enable_debug_mode();

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        engine->render();
    }

    mixer->destroy();
    engine->destroy();
}

auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}