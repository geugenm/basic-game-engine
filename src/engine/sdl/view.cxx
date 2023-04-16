#include "engine/engine.hxx"

#include <SDL3/SDL.h>
#include <iostream>

extern "C" class SDLView : public IView {
public:
    explicit SDLView() {}

    ~SDLView() override {
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

    void update() override {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_EVENT_QUIT) {
                break;
            }
        }
    }

    void render() override { SDL_RenderPresent(renderer_); }

private:
    static constexpr std::string_view kWindowTitle_{ "Hello SDL" };
    static constexpr int kWindowWidth_ = 640;
    static constexpr int kWindowHeight_ = 480;

    SDL_Window* window_;
    SDL_Renderer* renderer_;
};

IView* createView() {
    return new SDLView();
}
