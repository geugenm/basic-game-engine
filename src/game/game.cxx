#include "apps/application.h"
#include "engine/engine.hxx"

#include <iostream>

class ExampleGame : public Application {
public:
    explicit ExampleGame() {

    }

    ~ExampleGame() override {

    }

    void initialize() override {
        Engine::getInstance()->initialize();
    }

    void update() override {
        std::cout << "amazing!!";
        Engine::getInstance()->update();
    }

    void render() const override {
        Engine::getInstance()->render();
    }
};

Application* createApplication() {
    if (Engine::getInstance() != nullptr) {
        return new ExampleGame();
    }
    return nullptr;
}

void destroyApplication(Application* game) {
    delete game;
}
