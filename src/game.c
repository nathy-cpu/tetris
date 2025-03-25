#include "game.h"

#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "arena.h"
#include "array.h"
#include "block.h"
#include "colors.h"
#include "grid.h"

bool EventTriggered(double interval)
{
    static double lastUpdateTime = 0;
    double currentTime = GetTime();

    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }

    return false;
}

Game* Game_Init()
{
    Arena* arena = Arena_Init(MB128);

    if (arena == NULL)
        return NULL;

    Game* game = (Game*)Arena_AllocateAligned(arena, sizeof(Game));
    game->arena = arena;
    game->gameOver = false;
    game->score = 0;
    game->grid = Grid_Init(arena);
    game->blocks = ({
        static const char blockTypes[7] = { 'I', 'J', 'L', 'O', 'S', 'T', 'Z' };
        Array blocks = Array_Init(arena, 7, sizeof(Block));
        for (size_t i = 0; i < 7; i++) {
            Block block = *Block_Init(arena, blockTypes[i]);
            Array_Set(&blocks, i, &block);
        }

        blocks;
    });
    game->currentBlock = Game_GetRandomBlock(game);
    game->shadowBlock = game->currentBlock;
    game->nextBlock = Game_GetRandomBlock(game);
    InitAudioDevice();
    game->music = LoadMusicStream("assets/sounds/tetris-swing.wav");
    PlayMusicStream(game->music);
    game->rotateSound = LoadSound("assets/sounds/rotate.wav");
    game->clearSound = LoadSound("assets/sounds/clear.wav");
    game->moveSound = LoadSound("assets/sounds/move.wav");
    game->hardDropSound = LoadSound("assets/sounds/harddrop.wav");
    game->softDropSound = LoadSound("assets/sounds/softdrop.wav");
    game->font = LoadFont("assets/fonts/monogram.ttf");
    game->tileSpriteSheet = LoadTexture("assets/textures/tiles.png");
    SetTextureFilter(game->tileSpriteSheet, TEXTURE_FILTER_POINT);

    return game;
}

void Game_Close(Game* game)
{
    UnloadSound(game->rotateSound);
    UnloadSound(game->clearSound);
    UnloadSound(game->moveSound);
    UnloadSound(game->hardDropSound);
    UnloadSound(game->softDropSound);
    UnloadMusicStream(game->music);
    UnloadFont(game->font);
    UnloadTexture(game->tileSpriteSheet);
    CloseAudioDevice();
    Arena_Free(game->arena);
}

void Game_Update(Game* game)
{
    UpdateMusicStream(game->music);
    Game_HandleInput(game);

    if (EventTriggered(0.3))
        Game_MoveBlockDown(game);

    BeginDrawing();
    ClearBackground(darkBlue);
    DrawTextEx(game->font, "Score", (Vector2) { 365, 15 }, 38, 2, WHITE);
    DrawTextEx(game->font, "Next", (Vector2) { 370, 175 }, 38, 2, WHITE);

    if (game->gameOver)
        DrawTextEx(game->font, "GAME OVER", (Vector2) { 320, 450 }, 38, 2, WHITE);

    DrawRectangleRounded((Rectangle) { 320, 55, 170, 60 }, 0.3f, 6, lightBlue);

    char scoreText[10];
    sprintf(scoreText, "%d", game->score);
    Vector2 textSize = MeasureTextEx(game->font, scoreText, 38, 2);

    DrawTextEx(game->font, scoreText, (Vector2) { 320 + (170 - textSize.x) / 2, 65 }, 38, 2, WHITE);
    DrawRectangleRounded((Rectangle) { 320, 215, 170, 180 }, 0.3f, 6, lightBlue);
    Game_Draw(game);
    EndDrawing();
}

void Game_Draw(const Game* game)
{
    Grid_Draw(game->grid);
    Block_Draw(game->currentBlock, 11, 11);

    switch (game->nextBlock->id) {
    case 3:
        Block_Draw(game->nextBlock, 255, 290);
        break;
    case 4:
        Block_Draw(game->nextBlock, 255, 280);
        break;
    default:
        Block_Draw(game->nextBlock, 270, 270);
        break;
    }
}

void Game_HandleInput(Game* game)
{
    static float moveDelay = 0.0f;
    const float initialDelay = 0.3f;
    const float repeatDelay = 0.07f;
    float frameTime = GetFrameTime();

    int keyPressed = GetKeyPressed();

    if (game->gameOver && keyPressed != 0) {
        game->gameOver = false;
        Game_Reset(game);
    }

    switch (keyPressed) {
    case KEY_LEFT:
        Game_MoveBlockLeft(game);
        break;
    case KEY_RIGHT:
        Game_MoveBlockRight(game);
        break;
    case KEY_DOWN:
        Game_MoveBlockDown(game);
        break;
    case KEY_UP:
        Game_RotateBlock(game);
        break;
    }

    if (IsKeyDown(KEY_LEFT)) {
        moveDelay += frameTime;
        if (moveDelay >= initialDelay) {
            Game_MoveBlockLeft(game);
            moveDelay = initialDelay - repeatDelay;
        }
    } else if (IsKeyDown(KEY_RIGHT)) {
        moveDelay += frameTime;
        if (moveDelay >= initialDelay) {
            Game_MoveBlockRight(game);
            moveDelay = initialDelay - repeatDelay;
        }
    } else if (IsKeyDown(KEY_DOWN)) {
        moveDelay += frameTime;
        if (moveDelay >= initialDelay) {
            Game_MoveBlockDown(game);
            moveDelay = initialDelay - repeatDelay;
        }
    } else {
        // Reset the delay if no movement key is pressed
        moveDelay = 0.0f;
    }
}

void Game_MoveBlockDown(Game* game)
{
    if (!game->gameOver) {
        Block_Move(game->currentBlock, 1, 0);
        if (Game_IsBlockOutside(game) || Game_BlockFits(game) == false) {
            Block_Move(game->currentBlock, -1, 0);
            Game_LockBlock(game);
        }
    }
}

void Game_MoveBlockRight(Game* game)
{
    if (!game->gameOver) {
        Block_Move(game->currentBlock, 0, 1);
        if (Game_IsBlockOutside(game) || Game_BlockFits(game) == false)
            Block_Move(game->currentBlock, 0, -1);
    }
}

void Game_MoveBlockLeft(Game* game)
{
    if (!game->gameOver) {
        Block_Move(game->currentBlock, 0, -1);
        if (Game_IsBlockOutside(game) || Game_BlockFits(game) == false)
            Block_Move(game->currentBlock, 0, 1);
    }
}

Block* Game_GetRandomBlock(Game* game)
{
    if (game->blocks.data == NULL)
        game->blocks = Game_GetAllBlocks(game);

    int randomIndex = rand() % Array_Size(&game->blocks);

    return (Block*)Array_Get(&game->blocks, randomIndex);
}

Array Game_GetAllBlocks(Game* game)
{
    return game->blocks;
}

bool Game_IsBlockOutside(Game* game)
{
    Array tiles = Block_GetCellPositions(game->currentBlock);
    for (size_t i = 0; i < Array_Size(&tiles); i++) {
        Position position = *(Position*)Array_Get(&tiles, i);
        if (Grid_IsCellOutside(game->grid, position.row, position.column)) {
            Array_Free(&tiles);
            return true;
        }
    }
    Array_Free(&tiles);
    return false;
}

void Game_RotateBlock(Game* game)
{
    if (!game->gameOver) {
        Block_Rotate(game->currentBlock);

        if (Game_IsBlockOutside(game) || Game_BlockFits(game) == false)
            Block_UndoRotation(game->currentBlock);
        else
            PlaySound(game->rotateSound);
    }
}

void Game_LockBlock(Game* game)
{
    Array tiles = Block_GetCellPositions(game->currentBlock);

    for (size_t i = 0; i < Array_Size(&tiles); i++) {
        Position position = *(Position*)Array_Get(&tiles, i);
        game->grid->grid[position.row][position.column] = game->currentBlock->id;
    }

    game->currentBlock = game->nextBlock;

    if (Game_BlockFits(game) == false)
        game->gameOver = true;

    game->nextBlock = Game_GetRandomBlock(game);

    unsigned int rowsCleared = Grid_ClearFullRows(game->grid);
    if (rowsCleared > 0) {
        PlaySound(game->clearSound);
        Game_UpdateScore(game, rowsCleared, 0);
    }
    Array_Free(&tiles);
}

bool Game_BlockFits(Game* game)
{
    Array tiles = Block_GetCellPositions(game->currentBlock);

    for (size_t i = 0; i < Array_Size(&tiles); i++) {
        Position position = *(Position*)Array_Get(&tiles, i);
        if (Grid_IsEmpty(game->grid, position.row, position.column) == false) {
            Array_Free(&tiles);
            return false;
        }
    }

    Array_Free(&tiles);
    return true;
}

void Game_Reset(Game* game)
{
    Grid_Reset(game->grid);
    game->currentBlock = Game_GetRandomBlock(game);
    game->shadowBlock = Game_GetRandomBlock(game);
    game->nextBlock = Game_GetRandomBlock(game);
    game->score = 0;
}

void Game_UpdateScore(Game* game, uint32_t linesCleared, uint32_t moveDownPoints)
{
    game->score += moveDownPoints + linesCleared;
}
