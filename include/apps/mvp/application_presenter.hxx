#pragma once

#include "application_model.hxx"
#include "application_view.hxx"
#include "events/events.hxx"

#include <stdexcept>

class ApplicationPresenter {
public:
    explicit ApplicationPresenter() {
        model_ = create_application_model();
        view_  = create_application_view();
    }

    explicit ApplicationPresenter(ApplicationView*  view,
                                  ApplicationModel* model) {
        if (view == nullptr) {
            throw std::invalid_argument("Given view pointer is null");
        }

        if (model == nullptr) {
            throw std::invalid_argument("Given model pointer is null");
        }

        view_  = view;
        model_ = model;
    }

    virtual ~ApplicationPresenter() = default;

    virtual void initialize() = 0;
    virtual void run()        = 0;

protected:
    ApplicationView*  view_;
    ApplicationModel* model_;
};

extern "C" ApplicationPresenter* create_application_presenter(
    ApplicationView* view, ApplicationModel* model);
extern "C" void destroy_application_presenter(
    ApplicationPresenter* application_presenter);
