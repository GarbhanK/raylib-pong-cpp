#include "raylib.h"
#include "game.hpp"
#include "paddle.hpp"
#include "ball.hpp"

void resetPlayerScores(GameContext *ctx) {
    ctx->p1_score = 0;
    ctx->p2_score = 0;
}

void HandleGamestate(GameContext *ctx, Ball *b, Paddle *p1, Paddle *p2) {
    if (IsKeyPressed(KEY_TAB)) {
        if (ctx->debug) { ctx->debug = false; } else { ctx->debug = true; }
    }

    if (IsKeyPressed(KEY_ENTER)) {
        if (ctx->state == Gamestate::START) { ctx->state = Gamestate::PLAY; } else { ctx->state = Gamestate::START; }
        p1->reset(*ctx);
        p2->reset(*ctx);
        b->reset(ctx->center);
        resetPlayerScores(ctx);
    }

    if (IsKeyPressed(KEY_SPACE)) {
        if (ctx->state == Gamestate::PLAY) { ctx->state = Gamestate::PAUSE; } else { ctx->state = Gamestate::PLAY; }
    }
}

void handleBounce(GameContext ctx, Ball *ball, Paddle *p1, Paddle *p2) {
    // edge of play bounce
    if ( ball->pos.y <= (0 + static_cast<int>(ball->radius)) ) {
       ball->bounce(1, -1);
    }

    if ( ball->pos.y >= (ctx.screen_height - static_cast<int>(ball->radius)) ) {
        ball->bounce(1, -1);
    }

    // p1 paddle bounce
    if (
        (ball->pos.x <= p1->area.width + ball->radius) &&
        (ball->pos.y > p1->area.y) &&
        (ball->pos.y <= p1->area.y + p1->area.height)
    )
    {
        ball->bounce(-1, 1);
        ball->pos.x = ball->pos.x + 5;
    }

    // p2 paddle bounce
    if (
            (ball->pos.x >= ctx.screen_width - p2->area.width - ball->radius) &&
            (ball->pos.y >= p2->area.y) &&
            (ball->pos.y <= p2->area.y + p2->area.height)
        )
    {
        ball->bounce(-1, 1);
        ball->pos.x = ball->pos.x - 5;
    }
}

void handleScoring(GameContext *ctx, Ball *ball) {
    // scoring logic
    if (ball->pos.x <= 0) {
        ball->reset(ctx->center);
        ctx->p2_score += 1;
        if (ctx->p2_score == ctx->max_score) {
            ctx->state = Gamestate::WIN;
        }
    }
    if (ball->pos.x >= ctx->screen_width) {
        ball->reset(ctx->center);
        ctx->p1_score += 1;
        if (ctx->p1_score == ctx->max_score) {
            ctx->state = Gamestate::WIN;
        }
    }
}

void drawStart(GameContext ctx) {
    int title_len = MeasureText("PONG!", 50);
    int screen_width_int = static_cast<int>(ctx.center.x);
    DrawText("PONG!", screen_width_int - (title_len/2), 50, 50, BLACK);

    // draw controls instructions
    const char* p1Controls = "Player 1: press W or S to move the paddle";
    const char* p2Controls = "Player 2: press UP or DOWN to move the paddle";
    const char* startMsg = "Press ENTER to start the game!";
    int p1ControlsLen = MeasureText(p1Controls, 10);
    int p2ControlsLen = MeasureText(p2Controls, 10);
    int startMsgLen = MeasureText(startMsg, 10);

    DrawText(p1Controls, screen_width_int - (p1ControlsLen/2), 110, 10, BLACK);
    DrawText(p2Controls, screen_width_int - (p2ControlsLen/2), 130, 10, BLACK);
    DrawText(startMsg, screen_width_int - (startMsgLen/2), 150, 10, BLACK);
}

void drawPlay(GameContext ctx, Paddle p1, Paddle p2, Ball ball) {
    // draw paddles
    DrawRectangleRec(p1.area, RED);
    DrawRectangleRec(p2.area, BLUE);

    // draw ball
    DrawCircle(ball.pos.x, ball.pos.y, ball.radius, BLACK);

    int scorecardLen = MeasureText("P1: 0     p2: 0", 20);
    DrawText(
        TextFormat("P1: %i     p2: %i", ctx.p1_score, ctx.p2_score),
        static_cast<int>(ctx.center.x) - (scorecardLen/2), 10, 20, BLACK
    );

    DrawLine(ctx.center.x, 0, ctx.center.x, ctx.screen_height, BLACK);
    // TODO: other pong graphical bits, like a videoball type court
}

void drawPause(GameContext ctx, Paddle p1, Paddle p2, Ball ball) {
    // draw paddles (dimmed)
    DrawRectangleRec(p1.area, MAROON);
    DrawRectangleRec(p2.area, DARKBLUE);

    // draw ball
    DrawCircle(ball.pos.x, ball.pos.y, ball.radius, BLACK);
    int pauseLen = MeasureText("PAUSE", 50);
    DrawText("PAUSE", static_cast<int>(ctx.center.x) - pauseLen/2, ctx.center.y, 50, BLACK);
}

void drawWin(GameContext ctx) {
    // print win message
    const int winner = (ctx.p1_score > ctx.p2_score) ? 1 : 2;

    const char *winMsg = TextFormat("Player %d wins!", winner);
    const int winLen = MeasureText(winMsg, 50);
    DrawText(
        winMsg,
        static_cast<int>(ctx.center.x) - winLen/2,
        ctx.center.y,
        50,
        BLACK
    );
}
