#pragma once

#include "engine/interface/create_interfaces.hxx"
#include "interface/presenter.hxx"

#include <stdexcept>

namespace Game {

class Engine {
public:
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    static Engine& getInstance() {
        static Engine instance;
        return instance;
    }

    [[nodiscard]] IPresenter* getPresenter() const {
        if (!presenter_) {
            throw std::runtime_error("Presenter not set.");
        }
        return presenter_.get();
    }

    void setPresenter(std::unique_ptr<IPresenter> presenter) {
        if (!presenter) {
            throw std::invalid_argument("Presenter cannot be null.");
        }
        presenter_ = std::move(presenter);
    }

    void releasePresenter() {
        if (!presenter_) {
            return;
        }

        presenter_->setView(nullptr);
        presenter_->setModel(nullptr);
        presenter_.reset();
    }

    void formPresenter() {
        releasePresenter();
        presenter_ = std::unique_ptr<IPresenter>(createPresenter());
        presenter_->setView(createView());
        presenter_->setModel(createModel());
    }

private:
    Engine() {
        formPresenter();
    }

    virtual ~Engine() {
        releasePresenter();
    }

    std::unique_ptr<IPresenter> presenter_;
};

} // namespace Game
