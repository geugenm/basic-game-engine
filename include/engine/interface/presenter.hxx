#pragma once

#include <memory>

#include "model.hxx"
#include "view.hxx"

class IPresenter {
public:
    virtual ~IPresenter() {}
    virtual void initialize() = 0;

    virtual void update() = 0;
    virtual void render() = 0;

    virtual void setModel(std::shared_ptr<IModel> model) = 0;
    virtual void setView(std::shared_ptr<IView> view) = 0;
};

extern "C" IPresenter * create_presenter();
