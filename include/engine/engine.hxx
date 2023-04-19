#pragma once

#include "renderer/renderer.h"

#include <memory>
#include <stdexcept>

class Engine {
public:
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    static Engine* get_instance()
    {
        static Engine instance;
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

    void destroy() { release_renderer(); }

    [[nodiscard]] const IRenderer* getPresenter() const
    {
        return renderer_.get();
    }

    void setPresenter(std::unique_ptr<IRenderer> presenter)
    {
        if (presenter == nullptr) {
            throw std::invalid_argument("Presenter cannot be null.");
        }
        renderer_ = std::move(presenter);
    }

private:
    void release_renderer()
    {
        if (renderer_ == nullptr) {
            return;
        }

        renderer_->reset();
        renderer_.reset();
    }

    void form_renderer()
    {
        release_renderer();
        renderer_ = std::unique_ptr<IRenderer>(create_renderer());
    }

    Engine()
    {
        form_renderer();
    }

    virtual ~Engine()
    {
        release_renderer();
    }

    std::unique_ptr<IRenderer> renderer_;
};
