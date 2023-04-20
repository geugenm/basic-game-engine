#pragma once

class ApplicationModel {
public:
    virtual ~ApplicationModel() = default;
    virtual void initialize()   = 0;
};

extern "C" ApplicationModel* create_application_model();
extern "C" void destroy_application_model(ApplicationModel* application_model);
