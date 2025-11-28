#ifndef PADDLE_HPP
#define PADDLE_HPP

#include "raylib.h"
#include "game.hpp"

class Paddle {
private:
    const float paddle_speed;
    const Vector2 window;
public:
    Rectangle area;
    player_t player;

    Paddle(Rectangle area, player_t player, const float paddle_speed, const Vector2 window);
    void reset(GameContext ctx);
    void update(GameContext ctx, float delta);
};

#endif
