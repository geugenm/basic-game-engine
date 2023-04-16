#pragma once

#include "interface/presenter.hxx"

namespace Game {
    class EngineFactory {
    public:
        virtual ~EngineFactory() {}
        virtual std::shared_ptr<IModel> createModel();
        virtual std::shared_ptr<IPresenter> createPresenter();
        virtual std::shared_ptr<IView> createView();
    };
}

