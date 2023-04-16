#include "engine/engine.hxx"
#include "engine/misc/reload_attached_library.hxx"

int main() {
    Game::Engine::getInstance()->getPresenter()->initialize();
    bool a = true;
    while (true) {
        Game::Engine::getInstance()->getPresenter()->update();
        Game::Engine::getInstance()->getPresenter()->render();
//        if (a) {
//            reloadView("1234.so");
//            a = false;
//        }
    }

    return EXIT_SUCCESS;
}
