#pragma once

#include "engine/interface/presenter.hxx"

extern "C" IModel* createModel();
extern "C" IPresenter* createPresenter();
extern "C" IView* createView();
