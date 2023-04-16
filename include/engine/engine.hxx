#pragma once

#include "interface/presenter.hxx"

namespace Game {

class GameEngine {
public:
    GameEngine(GameEngine const&) = delete;
    GameEngine& operator=(GameEngine const&) = delete;

    ~GameEngine() {}

    static GameEngine* getInstance() {
        static GameEngine instance{};
        return &instance;
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

    std::shared_ptr<IPresenter> presenter_;
};

} // namespace Game
