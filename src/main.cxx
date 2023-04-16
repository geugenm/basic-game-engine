#include "engine/engine.hxx"
#include "engine/misc/reload_attached_library.hxx"

#include <chrono>
#include <iostream>
#include <thread>

int main() {
    Game::Engine::getInstance().getPresenter()->initialize();
    auto start = std::chrono::steady_clock::now();
    auto period = std::chrono::seconds(2);
    while (true) {
        Game::Engine::getInstance().getPresenter()->update();
        Game::Engine::getInstance().getPresenter()->render();
        auto now = std::chrono::steady_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
        if (duration >= period) {
            std::cout << "Passed 2 seconds!" << std::endl;
            reloadView("1234.so");
            start = now;
        }
        std::this_thread::sleep_for(
            std::chrono::milliseconds(16)); // sleep for 1/60 seconds
    }

    return EXIT_SUCCESS;
}
