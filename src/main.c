#include "tetris.h"

int main(void)
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
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