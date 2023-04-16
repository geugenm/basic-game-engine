#pragma once

#include "engine/interface/create_interfaces.hxx"
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

    [[nodiscard]] IPresenter * getPresenter() const {
        return presenter_;
    }

    void setPresenter(const std::shared_ptr<IPresenter>& presenter) {
        if (presenter == nullptr) {
            throw std::invalid_argument("Presenter cannot be null.");
        }
        presenter_ = presenter.get();
    }

private:
    explicit Engine() {
        presenter_ = createPresenter();
        presenter_->setView(createView());
        presenter_->setModel(createModel());
    }

    virtual ~Engine() {
        if (presenter_ == nullptr) {
            return;
        }

        presenter_->setView(nullptr);
        presenter_->setModel(nullptr);
        delete presenter_;
    }

    IPresenter * presenter_;
};

} // namespace Game
