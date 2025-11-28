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
    const float screen_width;
    const float screen_height;
    const Vector2 center;
    const int max_score;

    Gamestate state;
    int p1_score;
    int p2_score;
    bool debug;
};

void HandleGamestate(GameContext *ctx, Ball *b, Paddle *p1, Paddle *p2);
void handleBounce(GameContext ctx, Ball *ball, Paddle *p1, Paddle *p2);
void handleScoring(GameContext *ctx, Ball *ball);
void resetPlayerScores(GameContext *ctx);
void drawPlay(GameContext ctx, Paddle p1, Paddle p2, Ball ball);
void drawStart(GameContext ctx);
void drawPause(GameContext ctx, Paddle p1, Paddle p2, Ball ball);
void drawWin(GameContext ctx);

#endif
