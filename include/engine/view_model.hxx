#pragma once

#include "model.hxx"

class GameViewModel {
public:
    explicit GameViewModel(GameEngine& engine);

    void Init();

    void HandleInput();

    void Update();

    void Render();

private:
    GameEngine& engine_;
};
