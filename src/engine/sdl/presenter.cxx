#include "engine/engine.hxx"

class SDLPresenter final : public IPresenter {
public:
    explicit SDLPresenter() {
        model_ = createModel();
        view_ = createView();
    }

    ~SDLPresenter() override {
        delete model_;
        delete view_;
    }

    void initialize() override {
        view_->initialize();
    }

    void update() override {
        view_->update();
    }

    void render() override {
        view_->render();
    }

    void setModel(IModel* model) override {
        model_ = model;
    }

    IModel * getModel() override {
        return model_;
    }

    IView * getView() override {
        return view_;
    }

    void setView(IView* view) override {
        view_ = view;
    }
};

IPresenter* createPresenter() {
    return new SDLPresenter();
}
