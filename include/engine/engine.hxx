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

private:
    explicit GameEngine() {
        std::shared_ptr<IPresenter> presenter(create_presenter(), [](IPresenter* ptr) {
            delete ptr;
        });

        std::shared_ptr<IView> view(create_view(), [](IView* ptr) {
            delete ptr;
        });

        std::shared_ptr<IModel> model(create_model(), [](IModel* ptr) {
            delete ptr;
        });

        presenter_ = std::move(presenter);
        presenter_->setView(view);
        presenter_->setModel(model);
    }

    std::shared_ptr<IPresenter> presenter_;
};

} // namespace Game
