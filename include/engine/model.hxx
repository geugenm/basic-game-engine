#pragma once

#include "view_model.hxx"

#include <cstdint>
#include <vector>

// Forward declarations
class GameViewModel;
class GameView;

/**
 * @brief A simple game engine that follows an MVVM-like architecture
 */
class GameEngine {
public:
    /**
     * @brief Constructs a new GameEngine object
     *
     * @param windowWidth The width of the game window
     * @param windowHeight The height of the game window
     * @param title The title of the game window
     */
    GameEngine(uint16_t windowWidth, uint16_t windowHeight, const char* title);

    /**
     * @brief Runs the game loop
     */
    void Run();

private:
    /**
     * @brief Initializes SDL
     */
    void InitSDL();

    /**
     * @brief Initializes the game view model
     */
    void InitViewModel();

    /**
     * @brief Initializes the game view
     */
    void InitView();

    /**
     * @brief Processes input events
     */
    void ProcessInput();

    /**
     * @brief Updates the game state
     *
     * @param deltaTime The time elapsed since the last update
     */
    void Update(double deltaTime);

    /**
     * @brief Renders the game
     */
    void Render();

    uint16_t windowWidth_;
    uint16_t windowHeight_;
    const char* title_;

    bool isRunning_;

    GameViewModel* viewModel_;
    GameView* view_;

    std::vector<double> frameTimes_;
};
