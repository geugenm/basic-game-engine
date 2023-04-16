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

    virtual void setModel(IModel * model) = 0;
    virtual IModel * getModel() = 0;

    virtual void setView(IView * view) = 0;
    virtual IView * getView() = 0;

protected:
    IModel * model_;
    IView * view_;
};
