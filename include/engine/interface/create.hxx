#pragma once

#include "presenter.hxx"

extern "C" IModel* createModel();
extern "C" IPresenter* createPresenter();
extern "C" IView* createView();
