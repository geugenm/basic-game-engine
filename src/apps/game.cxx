#include "apps/mvp/mvp_application.hxx"
#include "engine.hxx"

#include <iostream>

class ExampleGame : public AbstractApplication {
public:
    explicit ExampleGame() { }

    ~ExampleGame() override { }

    void initialize() override { Engine::get_instance()->initialize(); }

    void update() override { Engine::get_instance()->update(); }

    void render() const override { Engine::get_instance()->render(); }
};

AbstractApplication* create_application() {
    if (Engine::get_instance() != nullptr) {
        return new ExampleGame();
    }
    return nullptr;
}

void destroy_application(AbstractApplication* application) {
    Engine::get_instance()->destroy();
    delete application;
}
