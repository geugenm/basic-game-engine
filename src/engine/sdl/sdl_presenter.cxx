#include "engine/interface/presenter.hxx"

class SDLPresenter : public IPresenter {
public:
    explicit SDLPresenter() {}

    void initialize() override {
        view_->initialize();
    }

    void update() override {
        view_->update();
    }

    void render() override {
        view_->render();
    }

    void setModel(std::shared_ptr<IModel> model) override { model_ = model; }

    void setView(std::shared_ptr<IView> view) override { view_ = view; }

private:
    std::shared_ptr<IModel> model_;
    std::shared_ptr<IView> view_;
};

IPresenter * create_presenter() {
    return new SDLPresenter();
}
