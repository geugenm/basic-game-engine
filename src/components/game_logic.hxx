#pragma once

#include <string>

namespace sdk
{

enum class game_topic : int
{
    history,
    evolution,
    zombies,
    mad_science
};

enum class game_genre : int
{
    action,
    adventure,
    rpg,
    simulations,
    strategy
};

struct genre_multiplier
{
    int _action;
    int _adventure;
    int _rpg;
    int _simulation;
    int _strategy;
};

struct game_platform
{
    std::string _name;
    genre_multiplier _genre_multiplier;
    uint16_t _market_share;
};

struct stage_1
{
    int _engine;
    int _game_play;
    int _story;
};

struct stage_2
{
    int _dialogues;
    int _level_design;
    int _ai;
};

struct stage_3
{
    int _world_design;
    int _graphics;
    int _sound;
};

struct game_pursuance
{
    int _bugs;
    int _art;
    int _engine;
    int _study;
};

struct game
{
    std::string _name;

    uint32_t _cost;

    game_topic _topic;
    game_genre _genre;
    game_platform _platform;

    stage_1 _stage_1;
    stage_2 _stage_2;
    stage_3 _stage_3;

    game_pursuance _pursuance;
};

} // namespace sdk
