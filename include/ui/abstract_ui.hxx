#pragma once

#include "events/events.hxx"

struct AbstractUI
{
    virtual ~AbstractUI()     = default;
    virtual void initialize() = 0;

    virtual void render() = 0;

    virtual void update() = 0;
    virtual void reset()  = 0;

    virtual Event get_last_event() = 0;
};

extern "C" AbstractUI* create_renderer();
extern "C" void        destroy_renderer(AbstractUI* renderer);
