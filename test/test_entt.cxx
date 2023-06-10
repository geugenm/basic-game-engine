#include <gengine_core.hxx> // include header file for the sdl_gl_engine class

int main(int argc, char *argv[], char *envp[])
{
    entt::registry registry;
    // create an instance of the game_system struct
    sdk::game_system system(registry, "My Window", 800, 600);

    // run the update method in a loop until the quit event occurs
    while (system.render_engine.sdl_context.is_initialized())
    {
        system.update(registry);
    }

    return EXIT_SUCCESS;
}
