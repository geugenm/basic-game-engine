#include <chrono>
#include <iostream>
#include <thread>
#include "apps/game.h"

int main() {
    Game * game = createGame();
    try {
        game->initialize();

        const auto period = std::chrono::seconds(2);
        auto nextFrameTime = std::chrono::steady_clock::now() + period;

        while (true) {
            game->update();
            game->render();

            const auto now = std::chrono::steady_clock::now();
            if (now >= nextFrameTime) {
                std::cout << "Passed 2 seconds!" << std::endl;
                nextFrameTime += period;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    } catch (const std::exception& ex) {
        std::cerr << "Unhandled exception: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
