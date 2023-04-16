#include "engine/engine.hxx"

class SDLModel : public IModel {
public:
    explicit SDLModel() {}

    void initialize() override {}

    void update() override {}

    void render() override {}
};

std::shared_ptr<IModel> Game::EngineFactory::createModel() {
    return std::make_shared<SDLModel>();
}
