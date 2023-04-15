#pragma once

#include "SDL3/SDL.h"

#include <cstdint>
#include <iostream>

class SDLApplication {
public:
    SDLApplication();

    ~SDLApplication();

    void Run();

private:
    constexpr static uint16_t kWindowWidth_  = 640;
    constexpr static uint16_t kWindowHeight_ = 480;

    SDL_Window*   window_;
    SDL_Renderer* renderer_;

    bool is_running_ = true;
};
