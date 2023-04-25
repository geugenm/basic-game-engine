/**
 * @file main.cpp
 *
 * @brief Entry point for the game application.
 */

#include "apps/mvp/mvp_application.hxx"

#include <chrono>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <thread>

/**
 * @brief Main function that runs the game application.
 *
 * @return Exit status of the application.
 */
int main()
{
    AbstractApplication *game = create_application();

    if (!game)
    {
        std::cerr << "Failed to create application instance." << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        game->initialize();
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Failed to initialize application: " << ex.what() << std::endl;
        destroy_application(game);
        return EXIT_FAILURE;
    }

    const auto framePeriod = std::chrono::seconds(3);
    auto nextFrameTime = std::chrono::steady_clock::now() + framePeriod;
    void *gameLibraryHandle{};

    bool isRunning = true;

    while (isRunning)
    {
        const auto now = std::chrono::steady_clock::now();
        if (now >= nextFrameTime)
        {
            std::cout << "10 seconds have passed!" << std::endl;
            nextFrameTime += framePeriod;
        }

        if (game != nullptr)
        {
            game->update();
            game->render();
        }
        else
        {
            game = create_application();
            game->initialize();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));

        if (!std::cout.good())
        {
            std::cerr << "Standard output stream is invalid." << std::endl;
            destroy_application(game);
            return EXIT_FAILURE;
        }
    }

    destroy_application(game);

    return EXIT_SUCCESS;
}
