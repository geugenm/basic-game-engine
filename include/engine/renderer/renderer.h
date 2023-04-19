#ifndef BASIC_GAME_ENGINE_RENDERER_H
#define BASIC_GAME_ENGINE_RENDERER_H

#include "../events/events.h"

struct Renderer {
    ~Renderer() = default;
    virtual void initialize() = 0;

    virtual void render() = 0;

    virtual void update() = 0;
    virtual void destroy() = 0;

    virtual Event getLastEvent() = 0;
};

#endif // BASIC_GAME_ENGINE_RENDERER_H
