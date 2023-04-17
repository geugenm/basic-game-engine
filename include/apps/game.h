#ifndef BASIC_GAME_ENGINE_GAME_H
#define BASIC_GAME_ENGINE_GAME_H

struct Game {
    virtual ~Game() = default;
    virtual void initialize() = 0;
    virtual void update() = 0;
    virtual void render() const = 0;
};

extern "C" Game* createGame();
extern "C" void destroyGame(Game * game);

#endif // BASIC_GAME_ENGINE_GAME_H
