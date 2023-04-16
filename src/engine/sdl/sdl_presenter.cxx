#include "engine/interface/presenter.hxx"

class SDLPresenter : public IPresenter {
public:
    void initialize() override {}

    void update() override {}

    void render() override {}

    void setModel(std::shared_ptr<IModel> model) override { model_ = model; }

    void setView(std::shared_ptr<IView> view) override { view_ = view; }

private:
    std::shared_ptr<IModel> model_;
    std::shared_ptr<IView> view_;
};
