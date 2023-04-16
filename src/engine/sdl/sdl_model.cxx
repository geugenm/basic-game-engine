#include "engine/interface/model.hxx"
#include "engine/engine.hxx"

class SDLModel : public IModel {
public:
    explicit SDLModel() {
        auto currentInstance = std::make_shared<SDLModel>(*this);
        Game::GameEngine::getInstance()->setModel(currentInstance);
    }

    void initialize() override {}

    void update() override {}

    void render() override {}
};
