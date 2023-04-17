#include "apps/application.h"
#include "engine/engine.hxx"

#include <iostream>

class RotatingTorDemo : public Application {
public:
    explicit RotatingTorDemo() {
    }

    ~RotatingTorDemo() override {

    }

    void initialize() override {
        Engine::getInstance()->initialize();
    }

    void update() override {
        Engine::getInstance()->update();
    }

    void render() const override {
        Engine::getInstance()->render();
    }
};

Application* createApplication() {
    return new RotatingTorDemo;
}

void destroyApplication(Application* application) {
    delete application;
}

