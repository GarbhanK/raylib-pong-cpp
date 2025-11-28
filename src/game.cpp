#include "raylib.h"
#include "game.hpp"
#include "paddle.hpp"
#include "ball.hpp"

void HandleGamestate(GameContext *ctx, Ball *b, Paddle *p1, Paddle *p2) {
    if (IsKeyPressed(KEY_TAB)) {
        if (ctx->debug) { ctx->debug = false; } else { ctx->debug = true; }
    }

    if (IsKeyPressed(KEY_ENTER)) {
        if (ctx->state == Gamestate::START) { ctx->state = Gamestate::PLAY; } else { ctx->state = Gamestate::START; }
        p1->reset(*ctx);
        p2->reset(*ctx);
        b->reset(ctx->center);
    }

    if (IsKeyPressed(KEY_SPACE)) {
        if (ctx->state == Gamestate::PLAY) { ctx->state = Gamestate::PAUSE; } else { ctx->state = Gamestate::PLAY; }
    }
}
