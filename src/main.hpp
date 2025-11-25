#include "raylib.h"

// declare classes
class Ball;
class Paddle;

// declare functions
void HandleGamestate(Ball *b, Paddle *p1, Paddle *p2);
void UpdatePaddle(Paddle *paddle, float dt);
void ResetPaddle(Paddle *p1, Paddle *p2);
Vector2 randomAngle();
