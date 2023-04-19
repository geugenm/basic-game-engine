#ifndef BASIC_GAME_ENGINE_RENDERER_H
#define BASIC_GAME_ENGINE_RENDERER_H

#include "../events/events.h"

struct Renderer {
    ~Renderer() = default;
    virtual void initialize() = 0;

    virtual void render() = 0;

    virtual void update() = 0;
    virtual void reset() = 0;

    virtual Event getLastEvent() = 0;
};

extern "C" Renderer * create_renderer();
extern "C" void destroy_renderer();

#endif // BASIC_GAME_ENGINE_RENDERER_H
