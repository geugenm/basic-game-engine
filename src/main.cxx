#include "engine/engine.hxx"

int main() {
    Game::GameEngine::getInstance()->getPresenter()->initialize();

    while (true) {
        Game::GameEngine::getInstance()->getPresenter()->update();
        Game::GameEngine::getInstance()->getPresenter()->render();
    }

    return EXIT_SUCCESS;
}
