#pragma once

#include "engine/interface/create.hxx"
#include "interface/presenter.hxx"

#include <stdexcept>

namespace Game {

class Engine {
public:
    Engine(Engine const&) = delete;
    Engine& operator=(Engine const&) = delete;

    static Engine* getInstance() {
        static Engine instance{};
        return &instance;
    }

    [[nodiscard]] const std::shared_ptr<IPresenter>& getPresenter() const {
        return presenter_;
    }

    void setPresenter(const std::shared_ptr<IPresenter>& presenter) {
        if (presenter == nullptr) {
            throw std::invalid_argument("Presenter cannot be null.");
        }
        presenter_ = presenter;
    }

private:
    explicit Engine() {
        auto presenter = createPresenter();
        auto model = createModel();
        auto view = createView();

        presenter_ = std::move(presenter);
        presenter_->setView(view);
        presenter_->setModel(model);
    }

    virtual ~Engine() {
        if (presenter_ == nullptr) {
            return;
        }

        presenter_->setView(nullptr);
        presenter_->setModel(nullptr);
        presenter_.reset();
    }

    std::shared_ptr<IPresenter> presenter_;
};

} // namespace Game
