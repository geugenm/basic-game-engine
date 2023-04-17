#include "apps/application.h"

#include <chrono>
#include <iostream>
#include <thread>

int main() {
    Application* game = createApplication();
    try {
        game->initialize();

        const auto period = std::chrono::seconds(2);
        auto nextFrameTime = std::chrono::steady_clock::now() + period;
        void * gameLibraryHandle{};

        while (true) {
            game->update();
            game->render();

            const auto now = std::chrono::steady_clock::now();
            if (now >= nextFrameTime) {
                std::cout << "Passed 2 seconds!" << std::endl;
                nextFrameTime += period;
                reloadApplicationLibrary(game, gameLibraryHandle);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    } catch (const std::exception& ex) {
        destroyApplication(game);
        std::cerr << "Unhandled exception: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    destroyApplication(game);

    return EXIT_SUCCESS;
}
