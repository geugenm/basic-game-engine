#include "apps/game.h"
#include "engine/engine.hxx"
#include <iostream>

class ExampleGame : public Game {
public:
    explicit ExampleGame() {

    }

    ~ExampleGame() override {

    }

    void initialize() override {
        Engine::getInstance()->initialize();
    }

    void update() override {
        std::cout << "wow!";
        Engine::getInstance()->update();
    }

    void render() const override {
        Engine::getInstance()->render();
    }
};

Game * createGame() {
    if (Engine::getInstance() != nullptr) {
        return new ExampleGame();
    }
    return nullptr;
}

void destroyGame(Game * game) {
    delete game;
}
