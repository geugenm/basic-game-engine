#include <gengine_core.hxx> // include header file for the sdl_gl_engine class

int main(int argc, char *argv[], char *envp[])
{
    entt::registry registry;
    // create an instance of the game_system struct
    sdk::game_system system(registry, "My Window", 1000, 1000);

    // run the update method in a loop until the quit event occurs
    while (system.render_engine.is_initialized(registry))
    {
        system.update(registry);
    }

    return EXIT_SUCCESS;
}
