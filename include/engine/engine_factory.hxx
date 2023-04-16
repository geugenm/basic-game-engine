#pragma once

#include "interface/presenter.hxx"

namespace Game {
    class EngineFactory {
    public:
        virtual ~EngineFactory() {}
        std::shared_ptr<IModel> createModel();
        std::shared_ptr<IPresenter> createPresenter();
        std::shared_ptr<IView> createView();
    };
}

