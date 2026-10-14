#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 680
#define BOARD_X 220
#define BOARD_Y 215
#define CELL_SIZE 120
#define BOARD_SIZE (CELL_SIZE * 3)

static const Color background = { 17, 20, 30, 255 };
static const Color panel = { 27, 32, 45, 255 };
static const Color cellHover = { 38, 45, 61, 255 };
static const Color gridLine = { 54, 62, 79, 255 };
static const Color mutedText = { 145, 155, 174, 255 };
static const Color brightText = { 241, 244, 250, 255 };
static const Color cyan = { 83, 220, 226, 255 };
static const Color pink = { 255, 117, 154, 255 };
static const Color green = { 112, 224, 164, 255 };

typedef struct Game {
    int board[9];
    int currentPlayer;
    int winner;
    int winningLine[3];
    int wins[3];
    int draws;
    bool gameOver;
} Game;

static void ResetRound(Game *game)
{
    for (int i = 0; i < 9; i++) game->board[i] = 0;
    game->currentPlayer = 1;
    game->winner = 0;
    game->gameOver = false;
    game->winningLine[0] = -1;
    game->winningLine[1] = -1;
    game->winningLine[2] = -1;
}

static bool FindWinner(Game *game)
{
    static const int lines[8][3] = {
        { 0, 1, 2 }, { 3, 4, 5 }, { 6, 7, 8 },
        { 0, 3, 6 }, { 1, 4, 7 }, { 2, 5, 8 },
        { 0, 4, 8 }, { 2, 4, 6 }
    };

    for (int i = 0; i < 8; i++)
    {
        int a = lines[i][0];
        int b = lines[i][1];
        int c = lines[i][2];

        if (game->board[a] != 0 &&
            game->board[a] == game->board[b] &&
            game->board[b] == game->board[c])
        {
            game->winner = game->board[a];
            game->winningLine[0] = a;
            game->winningLine[1] = b;
            game->winningLine[2] = c;
            game->gameOver = true;
            game->wins[game->winner]++;
            return true;
        }
    }

    for (int i = 0; i < 9; i++)
    {
        if (game->board[i] == 0) return false;
    }

    game->winner = -1;
    game->gameOver = true;
    game->draws++;
    return true;
}

static void DrawCenteredText(const char *text, int y, int fontSize, Color color)
{
    DrawText(text, (SCREEN_WIDTH - MeasureText(text, fontSize)) / 2, y, fontSize, color);
}

static void DrawMark(int mark, Vector2 center, float size, Color color)
{
    if (mark == 1)
    {
        float inset = size * 0.32f;
        DrawLineEx((Vector2){ center.x - inset, center.y - inset },
                   (Vector2){ center.x + inset, center.y + inset }, 9.0f, color);
        DrawLineEx((Vector2){ center.x + inset, center.y - inset },
                   (Vector2){ center.x - inset, center.y + inset }, 9.0f, color);
    }
    else if (mark == 2)
    {
        DrawRing(center, size * 0.25f, size * 0.34f, 0, 360, 64, color);
    }
}

static Vector2 CellCenter(int index)
{
    return (Vector2){
        BOARD_X + (index % 3) * CELL_SIZE + CELL_SIZE / 2.0f,
        BOARD_Y + (index / 3) * CELL_SIZE + CELL_SIZE / 2.0f
    };
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic Tac Toe");
    SetTargetFPS(60);

    Game game = { 0 };
    ResetRound(&game);

    const Rectangle newRoundButton = { 270, 596, 260, 48 };

    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();
        bool overBoard = CheckCollisionPointRec(mouse, (Rectangle){ BOARD_X, BOARD_Y, BOARD_SIZE, BOARD_SIZE });
        int hoveredCell = -1;

        if (overBoard)
        {
            int column = (int)(mouse.x - BOARD_X) / CELL_SIZE;
            int row = (int)(mouse.y - BOARD_Y) / CELL_SIZE;
            hoveredCell = row * 3 + column;
        }

        if (IsKeyPressed(KEY_R)) ResetRound(&game);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(mouse, newRoundButton))
            {
                ResetRound(&game);
            }
            else if (hoveredCell >= 0 && !game.gameOver && game.board[hoveredCell] == 0)
            {
                game.board[hoveredCell] = game.currentPlayer;

                if (!FindWinner(&game)) game.currentPlayer = 3 - game.currentPlayer;
            }
        }

        BeginDrawing();
        ClearBackground(background);

        DrawCenteredText("TIC TAC TOE", 20, 42, brightText);
        DrawCenteredText("CLASSIC  /  LOCAL MULTIPLAYER", 68, 15, mutedText);
        DrawRectangle(SCREEN_WIDTH / 2 - 30, 96, 60, 3, cyan);

        DrawRectangleRounded((Rectangle){ 180, 108, 440, 50 }, 0.22f, 12, panel);

        char status[64];
        Color statusColor = cyan;
        if (game.gameOver && game.winner > 0)
        {
            snprintf(status, sizeof(status), "%c takes the round!", game.winner == 1 ? 'X' : 'O');
            statusColor = game.winner == 1 ? cyan : pink;
        }
        else if (game.gameOver)
        {
            snprintf(status, sizeof(status), "It's a draw!");
            statusColor = green;
        }
        else
        {
            snprintf(status, sizeof(status), "%c's turn", game.currentPlayer == 1 ? 'X' : 'O');
            statusColor = game.currentPlayer == 1 ? cyan : pink;
        }

        DrawCenteredText(status, 120, 24, statusColor);

        DrawRectangleRounded((Rectangle){ 220, 166, 360, 36 }, 0.18f, 12, panel);
        char score[32];
        snprintf(score, sizeof(score), "X   %d", game.wins[1]);
        DrawText(score, 240, 174, 18, cyan);
        snprintf(score, sizeof(score), "DRAWS   %d", game.draws);
        DrawText(score, 350, 176, 16, mutedText);
        snprintf(score, sizeof(score), "%d   O", game.wins[2]);
        DrawText(score, 500, 174, 18, pink);

        DrawRectangleRounded((Rectangle){ BOARD_X - 8, BOARD_Y - 8, BOARD_SIZE + 16, BOARD_SIZE + 16 },
                             0.08f, 12, panel);

        for (int i = 0; i < 9; i++)
        {
            int column = i % 3;
            int row = i / 3;
            Rectangle cell = {
                BOARD_X + column * CELL_SIZE + 5,
                BOARD_Y + row * CELL_SIZE + 5,
                CELL_SIZE - 10,
                CELL_SIZE - 10
            };

            if (!game.gameOver && i == hoveredCell && game.board[i] == 0)
            {
                DrawRectangleRounded(cell, 0.08f, 8, cellHover);
                DrawMark(game.currentPlayer, CellCenter(i), (float)CELL_SIZE, Fade(statusColor, 0.4f));
            }

            DrawMark(game.board[i], CellCenter(i), (float)CELL_SIZE,
                     game.board[i] == 1 ? cyan : pink);
        }

        for (int i = 1; i < 3; i++)
        {
            DrawRectangle(BOARD_X + i * CELL_SIZE - 2, BOARD_Y + 8, 4, BOARD_SIZE - 16, gridLine);
            DrawRectangle(BOARD_X + 8, BOARD_Y + i * CELL_SIZE - 2, BOARD_SIZE - 16, 4, gridLine);
        }

        if (game.gameOver && game.winner > 0)
        {
            Vector2 start = CellCenter(game.winningLine[0]);
            Vector2 end = CellCenter(game.winningLine[2]);
            DrawLineEx(start, end, 7.0f, Fade(green, 0.9f));
        }

        bool overButton = CheckCollisionPointRec(mouse, newRoundButton);
        Color buttonColor = overButton ? (Color){ 63, 75, 97, 255 } : panel;
        DrawRectangleRounded(newRoundButton, 0.28f, 12, buttonColor);
        DrawRectangleRoundedLinesEx(newRoundButton, 0.28f, 12, 1.5f, gridLine);
        DrawCenteredText(game.gameOver ? "PLAY AGAIN" : "NEW GAME", 612, 17, brightText);
        DrawCenteredText("PRESS R TO RESTART", 656, 12, mutedText);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}