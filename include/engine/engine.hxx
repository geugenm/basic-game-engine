#pragma once

#include "interface/presenter.hxx"

namespace Game {

class GameEngine {
public:
    ~GameEngine() {}

    GameEngine(GameEngine& other) = delete;

    void operator=(const GameEngine&) = delete;

    static GameEngine* getInstance() {
        if (gameEngine_ == nullptr) {
            gameEngine_ = std::unique_ptr<GameEngine>(new GameEngine());
        }

        return gameEngine_.get();
    }

    [[nodiscard]] const std::shared_ptr<IPresenter>& getPresenter() const {
        return presenter_;
    }

    void setPresenter(const std::shared_ptr<IPresenter>& presenter) {
        presenter_ = presenter;
    }

    void setView(const std::shared_ptr<IView>& view) {
        presenter_->setView(view);
    }

    void setModel(const std::shared_ptr<IModel>& model) {
        presenter_->setModel(model);
    }

private:
    explicit GameEngine()
        : presenter_() {}

    static std::unique_ptr<GameEngine> gameEngine_;

    std::shared_ptr<IPresenter> presenter_;
};

} // namespace Game
