#pragma once

#include "ui/abstract_ui.hxx"

#include "runtime-loader/dll_reloader.hxx"

#include "render/colors/color_rgb.hxx"
#include "render/shapes/line_2d.hxx"
#include "render/shapes/box.hxx"

#include "render/shapes/indexed_shape.hxx"
#include "render/shapes/polygon_2d.hxx"
#include "render/shapes/position_2d.hxx"

#include "render/shapes/triangulated_shape.hxx"

#include "render/textures/ppm_handler.hxx"
#include "render/textures/texture.hxx"

#include <memory>
#include <stdexcept>

class Engine
{
  public:
    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    static Engine *get_instance()
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

    Engine()
    {
        form_renderer();
    }

    virtual ~Engine()
    {
        release_renderer();
    }

    std::unique_ptr<AbstractUI> renderer_;
};
