#pragma once

#include "interface/presenter.hxx"
#include "engine_factory.hxx"
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
        std::unique_ptr<EngineFactory> engineFactory(new EngineFactory());
        auto presenter = engineFactory->createPresenter();
        auto model = engineFactory->createModel();
        auto view = engineFactory->createView();

        presenter_ = std::move(presenter);
        presenter_->setView(view);
        presenter_->setModel(model);
    }

    ~Engine() {
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
