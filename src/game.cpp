#include "game.hpp"

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
