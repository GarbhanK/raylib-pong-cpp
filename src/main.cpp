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
        if (ball.pos.x <= 0) {
            ball.reset(CENTER);
            ctx.p2_score += 1;
            if (ctx.p2_score == MAX_SCORE) {
                ctx.state = Gamestate::WIN;
            }
        }
        if (ball.pos.x >= WIDTH) {
            ball.reset(CENTER);
            ctx.p1_score += 1;
            if (ctx.p1_score == MAX_SCORE) {
                ctx.state = Gamestate::WIN;
            }
        }

        // edge of play bounce
        if ( ball.pos.y <= (0 + static_cast<int>(ball.radius)) ) {
           ball.bounce(1, -1);
        }

        if ( ball.pos.y >= (HEIGHT - static_cast<int>(ball.radius)) ) {
            ball.bounce(1, -1);
        }

        // p1 paddle bounce
        if (
                (ball.pos.x <= p1.area.width + ball.radius) &&
                (ball.pos.y > p1.area.y) &&
                (ball.pos.y <= p1.area.y + p1.area.height)
            )
        {
            ball.bounce(-1, 1);
            ball.pos.x = ball.pos.x + 5;
        }

        // p2 paddle bounce
        if (
                (ball.pos.x >= WIDTH - p2.area.width - ball.radius) &&
                (ball.pos.y >= p2.area.y) &&
                (ball.pos.y <= p2.area.y + p2.area.height)
            )
        {
            ball.bounce(-1, 1);
            ball.pos.x = ball.pos.x - 5;
        }

        BeginDrawing();
            // set background colour
            ClearBackground(RAYWHITE);

            // std::string msg = "p1 score: " + std::to_string(p1.get_score());
            // std::cout << msg << std::endl;
            if (ctx.state == Gamestate::START) {
                // draw title
                int title_len = MeasureText("PONG!", 50);
                DrawText("PONG!", static_cast<int>(CENTER.x) - title_len/2, 50, 50, BLACK);

                // draw controls instructions
                const char* p1Controls = "Player 1: press W or S to move the paddle";
                const char* p2Controls = "Player 2: press UP or DOWN to move the paddle";
                const char* startMsg = "Press ENTER to start the game!";
                int p1ControlsLen = MeasureText(p1Controls, 10);
                int p2ControlsLen = MeasureText(p2Controls, 10);
                int startMsgLen = MeasureText(startMsg, 10);

                DrawText(p1Controls, static_cast<int>(CENTER.x) - (p1ControlsLen/2), 110, 10, BLACK);
                DrawText(p2Controls, static_cast<int>(CENTER.x) - (p2ControlsLen/2), 130, 10, BLACK);
                DrawText(startMsg, static_cast<int>(CENTER.x) - (startMsgLen/2), 150, 10, BLACK);

                // TODO: have black start screen, when start game shrink it to ball
                // TODO: add custom colorschemes/themes/courts
            }

            if (ctx.state == Gamestate::PLAY) {
                int scorecardLen = MeasureText("P1: 0     p2: 0", 20);
                DrawText(
                        TextFormat("P1: %i     p2: %i", ctx.p1_score, ctx.p2_score),
                        static_cast<int>(CENTER.x) - (scorecardLen/2), 10, 20, BLACK
                );

                DrawLine(CENTER.x, 0, CENTER.x, HEIGHT, BLACK);
                // TODO: other pong graphical bits, like a videoball type court
            }

            if (ctx.state == Gamestate::PAUSE) {
                int pauseLen = MeasureText("PAUSE", 50);
                DrawText("PAUSE", static_cast<int>(CENTER.x) - pauseLen/2, CENTER.y, 50, BLACK);
            }

            if (ctx.state == Gamestate::WIN) {
                // print win message
                const int winner = (ctx.p1_score > ctx.p2_score) ? 1 : 2;

                const char *winMsg = TextFormat("Player %d wins!", winner);
                const int winLen = MeasureText(winMsg, 50);
                DrawText(
                    winMsg,
                    static_cast<int>(CENTER.x) - winLen/2,
                    CENTER.y,
                    50,
                    BLACK
                );
            }

            if (ctx.debug) {
                DrawText(TextFormat("fps: %i", GetFPS()), 200, 10, 10, BLACK);
                DrawText(TextFormat("gamestate: %i", ctx.state), 200, 20, 10, BLACK);
                DrawText(TextFormat("ball speed: %0.2f", ball.speed), 200, 30, 10, BLACK);
                DrawLine(CENTER.x, 0, CENTER.x, HEIGHT, BLACK);
            }

			// draw objects
            if (ctx.state != Gamestate::WIN) {
                DrawRectangleRec(p1.area, RED);
                DrawRectangleRec(p2.area, BLUE);
                DrawCircle(ball.pos.x, ball.pos.y, ball.radius, BLACK);

                // TODO: add different map with extra collision bits
                //       iterate through array of recs and draw
                //       need extra collision code for them too
            }

        EndDrawing();
    }

	// cleanup
	CloseWindow();
	return 0;
}
