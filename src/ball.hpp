#ifndef BALL_HPP
#define BALL_HPP

#include "raylib.h"
#include "game.hpp"

class Ball {
public:
    Vector2 pos;
    Vector2 angle;
    float speed;
    const int radius;

    Ball(Vector2 pos, int radius, float speed);

    void update(GameContext ctx, float delta);
    void bounce(int x, int y);
    void reset(Vector2 pos);
    Vector2 random_angle();
};

#endif
