#pragma once

#include "presenter.hxx"

class InterfaceFactory {
public:
    virtual ~InterfaceFactory() {}
    virtual std::shared_ptr<IModel> createModel();
    virtual std::shared_ptr<IPresenter> createPresenter();
    virtual std::shared_ptr<IView> createView();
};
