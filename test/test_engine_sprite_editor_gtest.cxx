#include <gengine_core.hxx>
#include <gtest/gtest.h>

#include <thread>

TEST(GameSystemTest, UpdateAndHandleEvents)
{
    entt::registry main_registry;
    sdk::game_system system(main_registry, "My Window", 1920, 1080);

    while (system.is_initialized(main_registry))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 80));

        system.update(main_registry);

        system.handle_events(main_registry);
    }

    assert(system.is_initialized(main_registry) == false);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
