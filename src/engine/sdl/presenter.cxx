#include "engine/interface/presenter.hxx"

class SDLPresenter : public IPresenter {
public:
    void Initialize() override {
    }

    void Update(std::chrono::duration<double> deltaTime) override {
    }

    void Render() override {
    }

    void SetModel(std::shared_ptr<IModel> model) override {
        m_model = model;
    }

    void SetView(std::shared_ptr<IView> view) override {
        m_view = view;
    }

private:
    std::shared_ptr<IModel> m_model;
    std::shared_ptr<IView> m_view;
};
