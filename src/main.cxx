#include "engine/engine.hxx"

int main() {
    Game::Engine::getInstance()->getPresenter()->initialize();

    while (true) {
        Game::Engine::getInstance()->getPresenter()->update();
        Game::Engine::getInstance()->getPresenter()->render();
    }

    return EXIT_SUCCESS;
}
