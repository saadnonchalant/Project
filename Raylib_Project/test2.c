#include <raylib.h>

int main(void)
{
    InitWindow(800, 450, "raylib ball test");
    SetTargetFPS(60);

    Vector2 ball = { 400, 225 };
    float speed = 4.0f;

    while (!WindowShouldClose())
    {
        // Update
        if (IsKeyDown(KEY_RIGHT)) ball.x += speed;
        if (IsKeyDown(KEY_LEFT))  ball.x -= speed;
        if (IsKeyDown(KEY_DOWN))  ball.y += speed;
        if (IsKeyDown(KEY_UP))    ball.y -= speed;

        // Keep the ball inside the window
        if (ball.x < 20) ball.x = 20;
        if (ball.x > 780) ball.x = 780;
        if (ball.y < 20) ball.y = 20;
        if (ball.y > 430) ball.y = 430;

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Move the ball with the arrow keys", 10, 10, 20, DARKGRAY);
        DrawCircleV(ball, 20, MAROON);
        DrawFPS(10, 420);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}