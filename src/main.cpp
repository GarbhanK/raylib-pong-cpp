// #include "raylib.h"
#include "game.hpp"
#include "paddle.hpp"
#include "ball.hpp"
#include <raylib-cpp.hpp>

#define PADDLE_SPEED 400.0f
#define MAX_BALL_SPEED 5.0f
#define HEIGHT 600.0f
#define WIDTH 800.0f
#define MAX_SCORE 3

constexpr Vector2 WINDOW = { .x = WIDTH, .y = HEIGHT };
constexpr Vector2 CENTER = { WIDTH/2, HEIGHT/2 };


int main() {
	SetTargetFPS(60);
	// SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
	InitWindow(WIDTH, HEIGHT, "Raylib PONG");

    // initialise game state struct to start screen
    GameContext ctx = {
        .state = Gamestate::START,
        .p1_score = 0,
        .p2_score = 0,
        .debug = false,
        .screen_width = WIDTH,
        .screen_height = HEIGHT,
        .center = CENTER,
        .max_score = MAX_SCORE,
    };

	// Create paddles
    Paddle p1(
        (Rectangle){ 0, HEIGHT/2, 20, 110 },
        player_t::LEFT,
        PADDLE_SPEED,
        WINDOW
    );

	Paddle p2(
	    (Rectangle){ WIDTH-20, HEIGHT/2, 20, 110 },
		player_t::RIGHT,
		PADDLE_SPEED,
        WINDOW
	);

    // initialise the ball object
    Ball ball(CENTER, 20, 5.0f);

    // game loop
    while (!WindowShouldClose()) {
		float dt = GetFrameTime();

        HandleGamestate(&ctx, &ball, &p1, &p2); // controls START/PLAY/PAUSE logic

        p1.update(ctx, dt);
        p2.update(ctx, dt);
        ball.update(ctx, dt);

        // scoring logic
        handleScoring(&ctx, &ball);
        // if (ball.pos.x <= 0) {
        //     ball.reset(CENTER);
        //     ctx.p2_score += 1;
        //     if (ctx.p2_score == MAX_SCORE) {
        //         ctx.state = Gamestate::WIN;
        //     }
        // }
        // if (ball.pos.x >= WIDTH) {
        //     ball.reset(CENTER);
        //     ctx.p1_score += 1;
        //     if (ctx.p1_score == MAX_SCORE) {
        //         ctx.state = Gamestate::WIN;
        //     }
        // }

        // handle bounces against edges and paddles
        handleBounce(ctx, &ball, &p1, &p2);

        BeginDrawing();
            // set background colour
            ClearBackground(RAYWHITE);

            switch (ctx.state) {
               case Gamestate::START: drawStart(ctx); break;
               case Gamestate::PLAY: drawPlay(ctx); break;
               case Gamestate::PAUSE: drawPause(ctx); break;
               case Gamestate::WIN: drawWin(ctx); break;
            }

			// draw paddles and the ball when still in play
            if (ctx.state != Gamestate::WIN) {
                DrawRectangleRec(p1.area, RED);
                DrawRectangleRec(p2.area, BLUE);
                DrawCircle(ball.pos.x, ball.pos.y, ball.radius, BLACK);

                // TODO: add different map with extra collision bits
                //       iterate through array of recs and draw
                //       need extra collision code for them too
            }

            if (ctx.debug) {
                DrawText(TextFormat("fps: %i", GetFPS()), 200, 10, 10, BLACK);
                DrawText(TextFormat("gamestate: %i", ctx.state), 200, 20, 10, BLACK);
                DrawText(TextFormat("ball speed: %0.2f", ball.speed), 200, 30, 10, BLACK);
                DrawLine(CENTER.x, 0, CENTER.x, HEIGHT, BLACK);
            }

        EndDrawing();
    }

	// cleanup
	CloseWindow();
	return 0;
}
