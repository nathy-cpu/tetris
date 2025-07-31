#include "tetris.h"

int main(void)
{
    InitWindow(500, 620, "Tetris");
    SetTargetFPS(60);
    Game* game = Game_Init();

    while (!WindowShouldClose()) {
        Game_Update(game);
        Game_Draw(game);
    }

    Game_Close(game);
    CloseWindow();
    return 0;
}