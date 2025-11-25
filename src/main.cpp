#include "raylib.h"
#include "main.hpp"
#include <raylib-cpp.hpp>
#include <iostream>
#include <string>

#define PADDLE_SPEED 400.0f
#define MAX_BALL_SPEED 5.0f
#define HEIGHT 600.0f
#define WIDTH 800.0f
#define MAX_SCORE 3

constexpr Vector2 CENTER = { WIDTH/2, HEIGHT/2 };

class Paddle {
public:
    Rectangle area;
    player_t player;

    Paddle(Rectangle area, player_t player)
        : area(area), player(player) {}

    void reset()
    {
        // reset paddles positions to the middle of their range
        if (this->player == player_t::LEFT) {
            this->area.x = 0;
            this->area.y = HEIGHT/2;
        } else if (this->player == player_t::RIGHT) {
            this->area.x = WIDTH-20;
            this->area.y = HEIGHT/2;
        }
    }

    void update(GameContext ctx, float delta)
    {
        float currentPos = this->area.y;
        float targetPos = currentPos;

    	// paddle input
    	if ((this->player == player_t::RIGHT) && (ctx.state == Gamestate::PLAY)) {
            // handle upper/lower boundaries
            if ( IsKeyDown(KEY_UP) && (this->area.y >= 0) ) {
                targetPos -= PADDLE_SPEED*delta;
                this->area.y = Lerp(currentPos, targetPos, 1.0);
            }
    	    if (IsKeyDown(KEY_DOWN) && (this->area.y + this->area.height <= HEIGHT)) {
                targetPos += PADDLE_SPEED*delta;
                this->area.y = Lerp(currentPos, targetPos, 1.0);
            }
    	}

    	if ( (this->player == player_t::LEFT) && (ctx.state == Gamestate::PLAY) ) {
    		if (IsKeyDown(KEY_W) && (this->area.y >= 0) ) {
                targetPos -= PADDLE_SPEED*delta;
                this->area.y = Lerp(currentPos, targetPos, 1.0);
            }
    	    if (IsKeyDown(KEY_S) && (this->area.y + this->area.height <= HEIGHT)) {
                targetPos += PADDLE_SPEED*delta;
                this->area.y = Lerp(currentPos, targetPos, 1.0);
            }
    	}
    }
};

class Ball {
public:
    Vector2 pos;
    Vector2 angle;
    int radius;
    float speed;

    Ball(Vector2 pos, Vector2 angle, int radius, float speed)
        : pos(pos), angle(angle), radius(radius), speed(speed) {}

    void update(GameContext ctx, float delta)
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

    void bounce(int x, int y)
    {
        this->angle.x = x * this->angle.x;
        this->angle.y = y * this->angle.y;
        // TODO: play bounce SFX

        if (this->speed < 6.0f) {
            this->speed = this->speed + 0.2f;
        }
    }

    void reset(Vector2 random_angle)
    {
        // reset angle and set ball position to center of the screen
        this->pos = CENTER;
        this->angle = random_angle;
    }
};


int main ()
{
	SetTargetFPS(60);
	// SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
	InitWindow(WIDTH, HEIGHT, "Raylib PONG");

	// Create paddles
    Paddle P1(
        (Rectangle){ 0, HEIGHT/2, 20, 110 },
        player_t::LEFT
    );

	Paddle P2(
	    (Rectangle){ WIDTH-20, HEIGHT/2, 20, 110 },
		player_t::RIGHT
	);

    // initialise the ball object
    Ball ball(CENTER, randomAngle(), 20, 5.0f);

    // initialise game state struct to start screen
    GameContext ctx = {
        .state = Gamestate::START,
        .p1_score = 0,
        .p2_score = 0,
        .debug = false,
    };

    // game loop
    while (!WindowShouldClose())
	{
		float dt = GetFrameTime();

        HandleGamestate(&ctx, &ball, &P1, &P2); // controls START/PLAY/PAUSE logic

        P1.update(ctx, dt);
        P2.update(ctx, dt);
        ball.update(ctx, dt);

        // scoring logic
        if (ball.pos.x <= 0) {
            ball.reset(randomAngle());
            ctx.p2_score += 1;
            if (ctx.p2_score == MAX_SCORE) {
                ctx.state = Gamestate::WIN;
            }
        }
        if (ball.pos.x >= WIDTH) {
            ball.reset(randomAngle());
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

        // P1 paddle bounce
        if (
                (ball.pos.x <= P1.area.width + ball.radius) &&
                (ball.pos.y > P1.area.y) &&
                (ball.pos.y <= P1.area.y + P1.area.height)
            )
        {
            ball.bounce(-1, 1);
            ball.pos.x = ball.pos.x + 5;
        }

        // P2 paddle bounce
        if (
                (ball.pos.x >= WIDTH - P2.area.width - ball.radius) &&
                (ball.pos.y >= P2.area.y) &&
                (ball.pos.y <= P2.area.y + P2.area.height)
            )
        {
            ball.bounce(-1, 1);
            ball.pos.x = ball.pos.x - 5;
        }

        BeginDrawing();
            // set background colour
            ClearBackground(RAYWHITE);

            // std::string msg = "p1 score: " + std::to_string(P1.get_score());
            // std::cout << msg << std::endl;
            if (ctx.state == Gamestate::START) {
                // draw title
                int titleLen = MeasureText("PONG!", 50);
                DrawText("PONG!", static_cast<int>(CENTER.x) - titleLen/2, 50, 50, BLACK);

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
                int scorecardLen = MeasureText("P1: 0     P2: 0", 20);
                DrawText(
                        TextFormat("P1: %i     P2: %i", ctx.p1_score, ctx.p2_score),
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
                DrawRectangleRec(P1.area, RED);
                DrawRectangleRec(P2.area, BLUE);
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

Vector2 randomAngle() {
    Vector2 angle = {0};

    // dx, randomly choose either +100 or -100
    if (GetRandomValue(1, 2) == 1) {
        angle.x = 100.0f;
    } else {
        angle.x = -100.0f;
    };

    // dy, random value between -50 and 50
    angle.y = static_cast<float>(GetRandomValue(-50, 50));

    return angle;
}

void HandleGamestate(GameContext *ctx, Ball *b, Paddle *p1, Paddle *p2) {
    if (IsKeyPressed(KEY_TAB)) {
        if (ctx->debug) { ctx->debug = false; } else { ctx->debug = true; }
    }

    if (IsKeyPressed(KEY_ENTER)) {
        if (ctx->state == Gamestate::START) { ctx->state = Gamestate::PLAY; } else { ctx->state = Gamestate::START; }
        p1->reset();
        p2->reset();
        b->reset(randomAngle());
    }

    if (IsKeyPressed(KEY_SPACE)) {
        if (ctx->state == Gamestate::PLAY) { ctx->state = Gamestate::PAUSE; } else { ctx->state = Gamestate::PLAY; }
    }
}

