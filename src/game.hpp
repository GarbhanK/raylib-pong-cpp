#ifndef GAME_HPP
#define GAME_HPP

#include "raylib.h"

enum class Gamestate {
    START,
    PLAY,
    PAUSE,
    WIN
};

enum class player_t { LEFT, RIGHT };

struct GameContext {
    Gamestate state;
    int p1_score;
    int p2_score;
    bool debug;
    const float screen_width;
    const float screen_height;
};

Vector2 randomAngle();

#endif
