#include "engine/engine.hxx"

class SDLModel : public IModel {
public:
    explicit SDLModel() {}

    void initialize() override {}

    void update() override {}

    void render() override {}
};

IModel* createModel() {
    return new SDLModel();
}
