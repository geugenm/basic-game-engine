#pragma once

#include "ui/abstract_ui.hxx"

#include <memory>
#include <stdexcept>

class UIEngineSingleton
{
  public:
    UIEngineSingleton(const UIEngineSingleton &) = delete;
    UIEngineSingleton &operator=(const UIEngineSingleton &) = delete;

    static UIEngineSingleton *get_instance()
    {
        static UIEngineSingleton instance;
        return &instance;
    }

    void initialize()
    {
        renderer_->initialize();
    }

    void update()
    {
        renderer_->update();
    }

    void render()
    {
        renderer_->render();
    }

    void destroy()
    {
        release_renderer();
    }

    [[nodiscard]] const AbstractUI *get_presenter() const
    {
        return renderer_.get();
    }

    void set_presenter(std::unique_ptr<AbstractUI> presenter)
    {
        if (presenter == nullptr)
        {
            throw std::invalid_argument("Presenter cannot be null.");
        }
        renderer_ = std::move(presenter);
    }

  private:
    void release_renderer()
    {
        if (renderer_ == nullptr)
        {
            return;
        }

        renderer_->reset();
        renderer_.reset();
    }

    void form_renderer()
    {
        release_renderer();
        renderer_ = std::unique_ptr<AbstractUI>(create_renderer());
    }

    UIEngineSingleton()
    {
        form_renderer();
    }

    virtual ~UIEngineSingleton()
    {
        release_renderer();
    }

    std::unique_ptr<AbstractUI> renderer_;
};
