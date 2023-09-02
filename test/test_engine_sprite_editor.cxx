#include <gengine_core.hxx>

#include <thread>

int main(int argc, char **argv, char **envp)
{
    std::cout << "Launched with " << argc << " arguments." << std::endl;
    std::cout << "Parameters: " << argv[0] << std::endl;
    std::cout << "Set environment: " << envp[0] << std::endl;

    entt::registry main_registry;
    sdk::game_system system(main_registry, "My Window", 1920, 1080);

    while (system.is_initialized(main_registry))
    {
        system.update(main_registry);

        system.handle_events(main_registry);
    }

    int return_code = EXIT_SUCCESS;

    if (system.is_initialized(main_registry))
    {
        return_code = EXIT_FAILURE;
    }

    return return_code;
}