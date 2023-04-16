#include "engine/engine.hxx"

#include <chrono>
#include <iostream>
#include <thread>

int main() {
    try {
        auto& engine = Game::Engine::getInstance();
        engine.getPresenter()->initialize();

        const auto period = std::chrono::seconds(2);
        auto nextFrameTime = std::chrono::steady_clock::now() + period;

        while (true) {
            engine.getPresenter()->update();
            engine.getPresenter()->render();

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
