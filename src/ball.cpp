#include "raylib.h"
#include "raymath.h"
#include "game.hpp"
#include "ball.hpp"

Ball::Ball(Vector2 pos, int radius, float speed)
    : pos{ pos }
    , radius{ radius }
    , speed{ speed }
    , angle{ this->random_angle() }
{}

void Ball::update(GameContext ctx, float delta)
{
    if (ctx.state == Gamestate::PLAY) {
        Vector2 currentPos = this->pos;
        Vector2 targetPos = {
            currentPos.x + this->angle.x * (delta * this->speed),
            currentPos.y + this->angle.y * (delta * this->speed)
        };
        this->pos = Vector2Lerp(currentPos, targetPos, 1.0);
    }
}

void Ball::bounce(int x, int y)
{
    this->angle.x = x * this->angle.x;
    this->angle.y = y * this->angle.y;
    // TODO: play bounce SFX

    if (this->speed < 6.0f) {
        this->speed = this->speed + 0.2f;
    }
}

void Ball::reset(Vector2 pos)
{
    // reset angle and set ball position to center of the screen
    this->pos = pos;
    this->angle = this->random_angle();
}

Vector2 Ball::random_angle()
{
    Vector2 angle = {0};

    // dx, randomly choose either +100 or -100
    angle.x = GetRandomValue(1, 2) == 1 ? 100.0f : -100.0f;

    // dy, random value between -50 and 50
    angle.y = static_cast<float>(GetRandomValue(-50, 50));

    return angle;
}
