#include "sdl_ui.hxx"


AbstractUI* create_renderer()
{
    return new SDLUI();
}

void destroy_renderer(SDLUI* renderer)
{
    delete renderer;
}
