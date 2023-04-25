#pragma once

#include "events/events.hxx"

class ApplicationView {
public:
    virtual ~ApplicationView() = default;

    virtual void initialize()   = 0;
    virtual void update()       = 0;
    virtual void render() const = 0;

    [[nodiscard]] virtual Event get_latest_event() = 0;
};

extern "C" ApplicationView* create_application_view();
extern "C" void             destroy_application_view(ApplicationView* application_presenter);
