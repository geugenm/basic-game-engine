#pragma once

#include "apps/abstract_application.hxx"
#include "apps/mvp/application_presenter.hxx"

class AbstractMVPApplication : public AbstractApplication
{
public:
    explicit AbstractMVPApplication()
    {
        model_     = create_application_model();
        view_      = create_application_view();
        presenter_ = create_application_presenter(view_, model_);
    }

    ~AbstractMVPApplication() override = default;

protected:
    ApplicationPresenter* presenter_;
    ApplicationModel* model_;
    ApplicationView* view_;
};

extern "C" AbstractMVPApplication* create_mvp_application();
extern "C" void destroy_mvp_application(AbstractMVPApplication* application);
