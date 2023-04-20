#include "apps/application.h"
#include "engine.hxx"

#include <iostream>

class ExampleGame : public Application {
public:
    explicit ExampleGame() { }

    ~ExampleGame() override { }

    void initialize() override { Engine::get_instance()->initialize(); }

    void update() override { Engine::get_instance()->update(); }

    void render() const override { Engine::get_instance()->render(); }
};

Application* createApplication()
{
    if (Engine::get_instance() != nullptr) {
        return new ExampleGame();
    }
    return nullptr;
}

void destroyApplication(Application* application)
{
    Engine::get_instance()->destroy();
    delete application;
}
