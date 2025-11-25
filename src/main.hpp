#include "raylib.h"

// declare classes
class Ball;
class Paddle;

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
};

// declare functions
void HandleGamestate(GameContext *ctx, Ball *b, Paddle *p1, Paddle *p2);
void UpdatePaddle(Paddle *paddle, float dt);
void ResetPaddle(Paddle *p1, Paddle *p2);
Vector2 randomAngle();
