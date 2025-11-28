#include <cmath>
#include "raylib.h"
#include "paddle.hpp"

Paddle::Paddle(Rectangle area, player_t player, const float paddle_speed, const Vector2 window)
    : area{ area }
    , player{ player }
    , paddle_speed{ paddle_speed }
    , window{ window }
{
}

void Paddle::reset(GameContext ctx)
{
    // reset paddles positions to the middle of their range
    if (this->player == player_t::LEFT) {
        this->area.x = 0;
        this->area.y = ctx.screen_height/2;
    } else if (this->player == player_t::RIGHT) {
        this->area.x = ctx.screen_width-20;
        this->area.y = ctx.screen_height/2;
    }
}

void Paddle::update(GameContext ctx, float delta)
{
    float currentPos = this->area.y;
    float targetPos = currentPos;

   	// paddle input
   	if ((this->player == player_t::RIGHT) && (ctx.state == Gamestate::PLAY)) {
        // handle upper/lower boundaries
        if ( IsKeyDown(KEY_UP) && (this->area.y >= 0) ) {
            targetPos -= paddle_speed*delta;
            this->area.y = std::lerp(currentPos, targetPos, 0.9f);

        }
   	    if (IsKeyDown(KEY_DOWN) && (this->area.y + this->area.height <= window.y)) {
            targetPos += paddle_speed*delta;
            this->area.y = std::lerp(currentPos, targetPos, 0.9f);
        }
   	}

   	if ( (this->player == player_t::LEFT) && (ctx.state == Gamestate::PLAY) ) {
  		if (IsKeyDown(KEY_W) && (this->area.y >= 0) ) {
            targetPos -= paddle_speed*delta;
            this->area.y = std::lerp(currentPos, targetPos, 0.9f);
        }
   	    if (IsKeyDown(KEY_S) && (this->area.y + this->area.height <= window.y)) {
            targetPos += paddle_speed*delta;
            this->area.y = std::lerp(currentPos, targetPos, 0.9f);
        }
   	}
}
