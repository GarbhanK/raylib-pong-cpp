#ifndef GAME_HPP
#define GAME_HPP

#include "raylib.h"

// forward declarations to avoid circular imports
class Paddle;
class Ball;

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
    const Vector2 center;
};

void HandleGamestate(GameContext *ctx, Ball *b, Paddle *p1, Paddle *p2);

#endif
