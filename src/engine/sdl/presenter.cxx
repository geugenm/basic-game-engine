#include "engine/engine.hxx"

class SDLPresenter final : public IPresenter {
public:
    explicit SDLPresenter() {
        model_ = nullptr;
        view_ = nullptr;
    }

    ~SDLPresenter() override {
        delete model_;
        delete view_;
    }

    void initialize() override {
        if (view_ == nullptr) {
            throw std::invalid_argument("initialize(): View is nullptr");
        }
        view_->initialize();
    }

    void update() override {
        if (view_ == nullptr) {
            throw std::invalid_argument("update(): View is nullptr");
        }
        view_->update();
    }

    void render() override {
        if (view_ == nullptr) {
            throw std::invalid_argument("render(): View is nullptr");
        }
        view_->render();
    }

    void setModel(IModel* model) override {
        if (model == nullptr) {
            throw std::invalid_argument("Model pointer is null");
        }
        model_ = model;
    }

    void setView(IView* view) override {
        if (view == nullptr) {
            throw std::invalid_argument("View cannot be null");
        }
        view_ = view;
    }
};

IPresenter* createPresenter() {
    return new SDLPresenter();
}
