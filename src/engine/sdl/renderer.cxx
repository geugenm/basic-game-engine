#include "engine/renderer/renderer.h"

#include <SDL3/SDL.h>
#include <iostream>

class SDLRenderer : public IRenderer {
public:
    explicit SDLRenderer() = default;

    ~SDLRenderer() override {
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

    void initialize() override {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL initialization failed: " << SDL_GetError()
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }

        window_ = SDL_CreateWindow(
            kWindowTitle_.data(), kWindowWidth_, kWindowHeight_, 0);
        if (window_ == nullptr) {
            std::cerr << "Window creation failed: " << SDL_GetError()
                      << std::endl;
            SDL_Quit();
            std::exit(EXIT_FAILURE);
        }

        renderer_ = SDL_CreateRenderer(window_,
            nullptr,
            SDL_RENDERER_ACCELERATED |
                SDL_RENDERER_PRESENTVSYNC);
        if (renderer_ == nullptr) {
            std::cerr << "Renderer creation failed: " << SDL_GetError()
                      << std::endl;
            SDL_DestroyWindow(window_);
            SDL_Quit();
            std::exit(EXIT_FAILURE);
        }
    }

    void update() override {}

    void reset() override {
        renderer_ = nullptr;
        window_ = nullptr;
    }

    Event getLastEvent() override {
        SDL_Event event_;

        if (SDL_PollEvent(&event_) == 0) {
            return Event::NOTHING_HAPPENED;
        }

        switch (event_.type) {
        case SDL_EVENT_QUIT:
            return Event::WINDOW_CLOSE;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            switch (event_.button.button) {
            case SDL_BUTTON_LEFT:
                return Event::LEFT_PRESSED;
            case SDL_BUTTON_RIGHT:
                return Event::RIGHT_PRESSED;
            case SDL_BUTTON_MIDDLE:
                return Event::BUTTON1_PRESSED;
                // Add cases for other mouse buttons if needed
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            switch (event_.button.button) {
            case SDL_BUTTON_LEFT:
                return Event::LEFT_RELEASED;
            case SDL_BUTTON_RIGHT:
                return Event::RIGHT_RELEASED;
            case SDL_BUTTON_MIDDLE:
                return Event::BUTTON1_RELEASED;
                // Add cases for other mouse buttons if needed
            }
            break;
        case SDL_EVENT_KEY_DOWN:
            switch (event_.key.keysym.sym) {
            case SDLK_UP:
                return Event::UP_PRESSED;
            case SDLK_DOWN:
                return Event::DOWN_PRESSED;
            case SDLK_LEFT:
                return Event::LEFT_PRESSED;
            case SDLK_RIGHT:
                return Event::RIGHT_PRESSED;
            case SDLK_RETURN:
                return Event::SELECT_PRESSED;
            case SDLK_ESCAPE:
                return Event::START_PRESSED;
            case SDLK_a:
                return Event::BUTTON1_PRESSED;
            case SDLK_s:
                return Event::BUTTON2_PRESSED;
                // Add cases for other keys if needed
            }
            break;
        case SDL_EVENT_KEY_UP:
            switch (event_.key.keysym.sym) {
            case SDLK_UP:
                return Event::UP_RELEASED;
            case SDLK_DOWN:
                return Event::DOWN_RELEASED;
            case SDLK_LEFT:
                return Event::LEFT_RELEASED;
            case SDLK_RIGHT:
                return Event::RIGHT_RELEASED;
            case SDLK_RETURN:
                return Event::SELECT_RELEASED;
            case SDLK_ESCAPE:
                return Event::START_RELEASED;
            case SDLK_a:
                return Event::BUTTON1_RELEASED;
            case SDLK_s:
                return Event::BUTTON2_RELEASED;
                // Add cases for other keys if needed
            }
            break;
            // Add cases for other SDL event types if needed
        }

        return Event::NOTHING_HAPPENED;
    }

    void render() override { SDL_RenderPresent(renderer_); }

private:
    static constexpr std::string_view kWindowTitle_{ "Hello SDL" };
    static constexpr int kWindowWidth_ = 640;
    static constexpr int kWindowHeight_ = 480;

    SDL_Window* window_;
    SDL_Renderer* renderer_;
};

IRenderer* create_renderer() {
    return new SDLRenderer();
}

void destroy_renderer(SDLRenderer * renderer) {
    delete renderer;
}


