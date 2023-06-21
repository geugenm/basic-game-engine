#pragma once

#include "entt/entt.hpp"
#include <sdl_sound.hxx>

namespace sdk
{

struct audio_system
{
    sdl_subsdk::audio_mixer sound;

    audio_system(const char *sound_file_path) : sound(sound_file_path) {}

    void update(entt::registry &registry) {}
};

} // namespace sdk
