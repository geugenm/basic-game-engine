#pragma once

#include <entt/entt.hpp>
#include <sdl_sound.hxx>

namespace sdk
{

struct audio_system final
{
    sdl_subsdk::audio_mixer sound;

    explicit audio_system(const char *sound_file_path) : sound(sound_file_path)
    {
    }

    void update(entt::registry &registry) const {}
};

} // namespace sdk
