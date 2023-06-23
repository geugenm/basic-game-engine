#include <system/game_system.hxx>

int main(int argc, char *argv[])
{
    entt::registry main_registry;

    sdk::game_system system(main_registry, "My Window", 1920, 1080);

    while (system.is_initialized(main_registry))
    {
        system.update(main_registry);
        system.handle_events(main_registry);
    }

    return EXIT_SUCCESS;
}
