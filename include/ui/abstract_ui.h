#ifndef BASIC_GAME_ENGINE_RENDERER_H
#define BASIC_GAME_ENGINE_RENDERER_H

#include "../events/events.h"

struct AbstractUI {
    virtual ~AbstractUI() = default;
    virtual void initialize() = 0;

    virtual void render() = 0;

    virtual void update() = 0;
    virtual void reset() = 0;

    virtual Event get_last_event() = 0;
};

extern "C" AbstractUI* create_renderer();
extern "C" void destroy_renderer(AbstractUI* renderer);

#endif // BASIC_GAME_ENGINE_RENDERER_H
