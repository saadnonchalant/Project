#include <raylib.h>

int main(void)
{
InitWindow(800, 450, "raylib Test");
while (!WindowShouldClose())
{
BeginDrawing();
ClearBackground(RAYWHITE);
DrawText("raylib is working!", 250, 200, 30, DARKBLUE);
EndDrawing();
}
CloseWindow();
return 0;
}