#pragma once

#include "logger.hxx"
#include "sdl_functions.hxx"

#include <filesystem>

#include <chrono>
#include <cmath>

namespace sdl_subsdk
{
struct audio_buffer
{
    uint8_t *start     = nullptr;
    size_t size        = 0;
    size_t current_pos = 0;

    struct
    {
        size_t frequency = 0;
        double time      = 0.0;
        bool use_note    = false;
    } note;
};

enum class stereo_audio_channel
{
    left,
    right,
    left_and_right
};

static void audio_callback(void *userdata, uint8_t *stream, int len)
{
    static auto start             = std::chrono::high_resolution_clock::now();
    static auto finish            = start;
    static uint32_t default_delay = 0;
    static stereo_audio_channel channels{stereo_audio_channel::left_and_right};

    static bool first_time = true;
    if (first_time)
    {
        LOG(INFO) << "start audio_callback." << std::endl;
        first_time = false;
    }

    static constexpr int32_t AUDIO_FORMAT = SDL_AUDIO_S16LSB;

    // simulate long calculation (just test)
    SDL_Delay(default_delay);

    // experimental check two continuous callback delta time
    start  = finish;
    finish = std::chrono::high_resolution_clock::now();

    auto stream_len = static_cast<size_t>(len);
    // silence
    std::memset(stream, 0, static_cast<size_t>(len));

    auto audio_buff_data = static_cast<audio_buffer *>(userdata);

    if (audio_buff_data == nullptr)
    {
        throw std::invalid_argument("audio_buff_data is nullptr");
    }

    if (audio_buff_data->note.frequency != 0)
    {
        size_t num_samples = stream_len / 2 / 2;
        double dt          = 1.0 / 48000.0;

        auto output = reinterpret_cast<int16_t *>(stream);

        for (size_t sample = 0; sample < num_samples; ++sample)
        {
            double omega_t =
                audio_buff_data->note.time * 2.0 * 3.1415926 *
                static_cast<double>(audio_buff_data->note.frequency);
            double curr_sample =
                65535 * sin(omega_t);
            auto curr_val = static_cast<int16_t>(curr_sample);

            switch (channels)
            {
                case stereo_audio_channel::left_and_right: {
                    *output = curr_val;
                    output++;
                    *output = curr_val;
                    output++;
                    break;
                }
                case stereo_audio_channel::left: {
                    *output = curr_val;
                    output++;
                    *output = 0;
                    output++;
                    break;
                }
                case stereo_audio_channel::right: {
                    *output = 0;
                    output++;
                    *output = curr_val;
                    output++;
                    break;
                }
                default:
                    break;
            }

            audio_buff_data->note.time += dt;
        }
    }
    else
    {
        while (stream_len > 0)
        {
            // copy data from loaded buffer into output stream
            const uint8_t *current_sound_pos =
                audio_buff_data->start + audio_buff_data->current_pos;

            const size_t left_in_buffer =
                audio_buff_data->size - audio_buff_data->current_pos;

            if (left_in_buffer > stream_len)
            {
                SDL_MixAudioFormat(stream, current_sound_pos, AUDIO_FORMAT,
                                   static_cast<Uint32>(len), 128);
                audio_buff_data->current_pos += stream_len;
                break;
            }
            else
            {
                // first copy rest from buffer and repeat sound from beginning
                SDL_MixAudioFormat(stream, current_sound_pos, AUDIO_FORMAT,
                                   static_cast<Uint32>(left_in_buffer), 128);
                // start buffer from beginning
                audio_buff_data->current_pos = 0;
                stream_len -= left_in_buffer;
            }
        }
    }
}

class audio_mixer
{
public:
    explicit audio_mixer(const char *sound_file_name)
        : sound_file_name_(sound_file_name)
    {
        if (sound_file_name_ == nullptr)
        {
            throw std::invalid_argument("Sound file is nullptr");
        }

        initialize();
    }

    ~audio_mixer() = default;

    void change_sound(const char *sound_file_name)
    {
        if (sound_file_name_ == nullptr)
        {
            throw std::invalid_argument("Sound file is nullptr");
        }
        sound_file_name_ = sound_file_name;
        initialize();
    }

    void initialize()
    {
        LOG(INFO) << "Initializing audio mixer";

        if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
            LOG(ERROR) << "Can't init audio: " << SDL_GetError();
            throw std::invalid_argument("Failed to initialize SDL audio");
        }

        LOG(INFO) << "SDL audio initialized";

        LOG(INFO) << "Loading sound file: " << sound_file_name_;

        sound_file_ = SDL_RWFromFile(sound_file_name_, "rb");
        if (sound_file_ == nullptr)
        {
            LOG(ERROR) << "Can't open file: " << sound_file_name_;
            throw std::invalid_argument(
                "Failed to open sound file '" + std::string(sound_file_name_) +
                "', current directory: " +
                std::filesystem::current_path().string());
        }

        LOG(INFO) << "Loaded sound file: " << sound_file_name_;

        LOG(INFO) << "Loading audio spec from file: " << sound_file_name_;

        audio_spec_ = SDL_LoadWAV_RW(
            sound_file_, auto_delete_file_, &audio_spec_from_file_,
            &sample_buffer_from_file_, &sample_buffer_len_from_file_);

        if (audio_spec_ == nullptr)
        {
            LOG(INFO) << "Can't parse audio spec from file: "
                      << sound_file_name_;
            throw std::invalid_argument("Failed to parse audio spec");
        }

        LOG(INFO) << "Loaded audio spec from file: " << sound_file_name_;

        loaded_audio_buff_ = {
            .start       = sample_buffer_from_file_,
            .size        = sample_buffer_len_from_file_,
            .current_pos = 0,
            .note        = {.frequency = 0, .time = 0, .use_note = false}};

        LOG(INFO) << "Audio buffer from file size: "
                  << sample_buffer_len_from_file_ << " B ("
                  << sample_buffer_len_from_file_ / double(1024 * 1024)
                  << ") MB";

        const char *device_name         = nullptr; // device name or nullptr
        const int32_t is_capture_device = 0; // 0 - play device, 1 - microphone
        SDL_AudioSpec desired{.freq     = 48000,
                              .format   = AUDIO_FORMAT,
                              .channels = 2,    // stereo
                              .silence  = 0,
                              .samples  = 4096, // must be power of 2
                              .padding  = 0,
                              .size     = 0,
                              .callback = audio_callback,
                              .userdata = &loaded_audio_buff_};

        SDL_AudioSpec returned{};

        const int32_t allow_changes = 0;

        audio_device_id_ = SDL_OpenAudioDevice(
            device_name, is_capture_device, &desired, &returned, allow_changes);
        if (audio_device_id_ == 0)
        {
            LOG(INFO) << "Failed to open audio device: " << SDL_GetError();
            throw std::invalid_argument("Failed to open audio device");
        }

        LOG(INFO) << "Audio device opened: " << audio_device_id_;

        if (desired.format != returned.format ||
            desired.channels != returned.channels ||
            desired.freq != returned.freq)
        {
            LOG(INFO) << "Audio device settings are not equal:";
            throw std::invalid_argument("Failed to open audio device");
        }

        // start playing audio thread
        // now callback is firing
        SDL_PlayAudioDevice(audio_device_id_);

        LOG(INFO) << "Audio device started";
    }

    void destroy()
    {
        LOG(INFO) << "Pause audio device (stop audio thread)";

        SDL_PauseAudioDevice(audio_device_id_);

        LOG(INFO) << "Close audio device";

        SDL_CloseAudioDevice(audio_device_id_);

        SDL_free(loaded_audio_buff_.start);

        SDL_Quit();
    }

    static constexpr int32_t AUDIO_FORMAT = SDL_AUDIO_S16LSB;

private:
    SDL_RWops *sound_file_{};

    SDL_AudioSpec audio_spec_from_file_{};
    const int32_t auto_delete_file_       = 1;
    uint8_t *sample_buffer_from_file_     = nullptr;
    uint32_t sample_buffer_len_from_file_ = 0;

    SDL_AudioDeviceID audio_device_id_{};

    SDL_AudioSpec const *audio_spec_{};

    audio_buffer loaded_audio_buff_;

    const char *sound_file_name_;
};
} // namespace sdl_subsdk