#pragma once

#include "engine/events/events.hxx"

class IView {
public:
    virtual ~IView() = default;

    virtual void initialize() = 0;

    virtual void update() = 0;
    virtual void render() = 0;
    virtual void destroy() = 0;

    virtual Event getLastEvent() = 0;
};
