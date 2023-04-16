#pragma once

#include <chrono>
#include <memory>

#include "model.hxx"
#include "view.hxx"

class IPresenter {
public:
    virtual ~IPresenter() {}
    virtual void Initialize() = 0;

    virtual void Update(std::chrono::duration<double> deltaTime) = 0;
    virtual void Render() = 0;

    virtual void SetModel(std::shared_ptr<IModel> model) = 0;
    virtual void SetView(std::shared_ptr<IView> view) = 0;
};
