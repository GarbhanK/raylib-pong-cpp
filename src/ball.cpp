#include "raylib.h"
#include "raymath.h"
#include "game.hpp"
#include "ball.hpp"

Ball::Ball(Vector2 pos, Vector2 angle, int radius, float speed)
    : pos(pos), angle(angle), radius(radius), speed(speed) {}

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

void Ball::reset(Vector2 pos, Vector2 random_angle)
{
    // reset angle and set ball position to center of the screen
    this->pos = pos;
    this->angle = random_angle;
}
