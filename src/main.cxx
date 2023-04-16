#include "engine/engine.hxx"

int main() {
    create_presenter();
    create_model();
    create_view();

    Game::GameEngine::getInstance()->getPresenter()->initialize();

    while (true) {
        Game::GameEngine::getInstance()->getPresenter()->update();
        Game::GameEngine::getInstance()->getPresenter()->render();
    }

    return EXIT_SUCCESS;
}
