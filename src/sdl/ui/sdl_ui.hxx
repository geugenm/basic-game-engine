#pragma once

#include "ui/abstract_ui.hxx"

#include "SDL3/SDL.h"
#include <iostream>

class SDLUI : public AbstractUI {
public:
    explicit SDLUI() = default;

    ~SDLUI() override;

    void initialize() override;

    void update() override;

    void reset() override;

    Event get_last_event() override;

    void render() override;

private:
    static constexpr std::string_view kWindowTitle_ { "Hello SDL" };
    static constexpr int kWindowWidth_ = 640;
    static constexpr int kWindowHeight_ = 480;

    SDL_Window* window_;
    SDL_Renderer* renderer_;
};


