#include "test_window.hxx"
/**
 * @file main.hxx
 * @brief The main file that contains the entry point for the application
 */

/**
 * @brief The main entry point of the application
 *
 * @param argc The number of command-line arguments
 * @param argv The list of command-line arguments
 * @return The exit status of the application
 */
auto main() -> int {
    SDLApplication app;
    app.Run();
    return EXIT_SUCCESS;
}

/**
 * @brief The constructor of the SDLApplication class that initializes SDL
 * components
 */
SDLApplication::SDLApplication() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError()
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }

    window_ = SDL_CreateWindow("Hello SDL", kWindowWidth_, kWindowHeight_, 0);
    if (window_ == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        std::exit(EXIT_FAILURE);
    }

    renderer_ = SDL_CreateRenderer(
        window_, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer_ == nullptr) {
        std::cerr << "Renderer creation failed: " << SDL_GetError()
                  << std::endl;
        SDL_DestroyWindow(window_);
        SDL_Quit();
        std::exit(EXIT_FAILURE);
    }
}

/**
 * @brief The destructor of the SDLApplication class that releases the SDL
 * components
 */
SDLApplication::~SDLApplication() {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

/**
 * @brief The main loop of the application that runs until it's stopped by the
 * user
 */
void SDLApplication::Run() {
    while (is_running_) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_EVENT_QUIT) {
                is_running_ = false;
                break;
            }
        }

        SDL_RenderPresent(renderer_);
    }
}
