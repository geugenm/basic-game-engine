#include "apps/application.h"

#include <chrono>
#include <iostream>
#include <thread>

int main() {
    Application* game{ createApplication() };

    try {
        game->initialize();
    } catch (const std::exception& ex) {
        std::cerr << "Failed to initialize application: " << ex.what()
                  << std::endl;
        return EXIT_FAILURE;
    }

    const auto framePeriod = std::chrono::seconds(10);
    auto nextFrameTime = std::chrono::steady_clock::now() + framePeriod;
    void* gameLibraryHandle{};

    while (true) {
        const auto now = std::chrono::steady_clock::now();
        if (now >= nextFrameTime) {
            std::cout << "10 seconds have passed!" << std::endl;
            nextFrameTime += framePeriod;
            game = reloadApplicationLibrary(game, gameLibraryHandle);
        }

        if (game) {
            game->update();
            game->render();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    destroyApplication(game);
    return EXIT_SUCCESS;
}
