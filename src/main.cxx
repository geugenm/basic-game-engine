#include "engine/engine.hxx"
#include "engine/misc/reload_attached_library.hxx"

int main() {
    Game::Engine::getInstance()->getPresenter()->initialize();

    while (true) {
        Game::Engine::getInstance()->getPresenter()->update();
        Game::Engine::getInstance()->getPresenter()->render();
        reloadView("1234.so");
    }

    return EXIT_SUCCESS;
}
