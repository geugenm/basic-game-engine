#pragma once

#include "interface/presenter.hxx"

class GameEngine {

protected:
    static std::unique_ptr<GameEngine> gameEngine_;

public:
    GameEngine(GameEngine &other) = delete;

    void operator=(const GameEngine &) = delete;

    static GameEngine *GetInstance(const std::string& value) {
        if(gameEngine_==nullptr){
            gameEngine_ = new GameEngine(value);
        }
        return gameEngine_;
    }

    void SomeBusinessLogic()
    {
        // ...
    }

    std::string value() const{
        return value_;
    }
};
