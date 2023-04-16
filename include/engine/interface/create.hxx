#pragma once

#include "presenter.hxx"

std::shared_ptr<IModel> createModel();
std::shared_ptr<IPresenter> createPresenter();
std::shared_ptr<IView> createView();
