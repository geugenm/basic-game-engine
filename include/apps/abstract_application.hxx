#pragma once

class AbstractApplication {
public:
    virtual ~AbstractApplication() = default;

    virtual void initialize()   = 0;
    virtual void update()       = 0;
    virtual void render() const = 0;
};

extern "C" AbstractApplication* create_application();
extern "C" void                 destroy_application(AbstractApplication* application);
