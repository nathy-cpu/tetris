#include "tetris.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool EventTriggered(double* lastUpdateTime, const double interval)
{
    const double currentTime = GetTime();
    if (currentTime - *lastUpdateTime >= interval) {
        *lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

Game* Game_Init()
{
    Game* game = malloc(sizeof(Game));
    assert(game != NULL);
    game->gameOver = false;
    game->score = 0;
    game->numBlocks = NUM_BLOCKS;
    game->board = Board_Init();

    // Spawn initial blocks
    game->currentBlock = GetRandomBlock();
    game->nextBlock = GetRandomBlock();
    game->shadowBlock = Block_Clone(game->currentBlock);

    // Initialize audio and graphics
    InitAudioDevice();
    game->music = LoadMusicStream("assets/sounds/tetris-swing.wav");
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
    Block_Free(game->currentBlock);
    Block_Free(game->nextBlock);
    Block_Free(game->shadowBlock);
    Board_Free(game->board);

    UnloadSound(game->rotateSound);
    UnloadSound(game->clearSound);
    UnloadSound(game->moveSound);
    UnloadSound(game->hardDropSound);
    UnloadSound(game->softDropSound);
    StopMusicStream(game->music);
    UnloadMusicStream(game->music);

    UnloadFont(game->font);
    UnloadTexture(game->tileSpriteSheet);
    CloseAudioDevice();
    free(game);
}

void Game_Update(Game* game)
{
    static double dropTimer = 0;
    Game_UpdateShadowBlock(game);
    Game_HandleInput(game);

    if (game->gameOver) {
        if (IsMusicStreamPlaying(game->music))
            StopMusicStream(game->music);
    } else {
        if (!IsMusicStreamPlaying(game->music))
            PlayMusicStream(game->music);

        UpdateMusicStream(game->music);
    }

    if (EventTriggered(&dropTimer, 0.3))
        Game_MoveBlockDown(game);
}

void Game_Draw(const Game* game)
{
    assert(game->board != NULL);
    assert(game->currentBlock != NULL);
    assert(game->nextBlock != NULL);
    BeginDrawing();
    ClearBackground(darkBlue);
    DrawTextEx(game->font, "Score", (Vector2) { 365, 15 }, FONT_SIZE, FONT_SPACING, WHITE);
    DrawTextEx(game->font, "Next", (Vector2) { 370, 175 }, FONT_SIZE, FONT_SPACING, WHITE);

    if (game->gameOver) {
        DrawTextEx(game->font, "GAME OVER", (Vector2) { 320, 450 }, FONT_SIZE, FONT_SPACING, WHITE);
    }

    DrawRectangleRounded((Rectangle) { 320, 55, 170, 60 }, 0.3f, 6, lightBlue);

    char scoreText[10];
    sprintf(scoreText, "%d", game->score);
    const Vector2 textSize = MeasureTextEx(game->font, scoreText, FONT_SIZE, FONT_SPACING);

    DrawTextEx(game->font, scoreText, (Vector2) { 320 + (170 - textSize.x) / 2, 65 }, FONT_SIZE, FONT_SPACING, WHITE);
    DrawRectangleRounded((Rectangle) { 320, 215, 170, 180 }, 0.3f, 6, lightBlue);
    Board_Draw(game->board, game->tileSpriteSheet);
    Block_Draw(game->currentBlock, 11, 11, game->tileSpriteSheet, 1.0);
    Block_Draw(game->shadowBlock, 11, 11, game->tileSpriteSheet, 0.2);

    switch (game->nextBlock->id) {
    case 3:
        Block_Draw(game->nextBlock, 255, 290, game->tileSpriteSheet, 1.0);
        break;
    case 4:
        Block_Draw(game->nextBlock, 255, 280, game->tileSpriteSheet, 1.0);
        break;
    default:
        Block_Draw(game->nextBlock, 270, 270, game->tileSpriteSheet, 1.0);
        break;
    }
    EndDrawing();
}

void Game_HandleInput(Game* game)
{
    const int keyPressed = GetKeyPressed();

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
        Game_DropBlock(game);
        break;
    case KEY_UP:
        Game_RotateBlock(game);
        break;
    default:
        break;
    }
}

void Game_MoveBlockDown(Game* game)
{
    if (!game->gameOver) {
        Block_Move(game->currentBlock, (Position) { 1, 0 });
        if (Game_IsBlockOutside(game) || Game_BlockFits(game) == false) {
            Block_Move(game->currentBlock, (Position) { -1, 0 });
            Game_LockBlock(game);
        }
    }
}

void Game_DropBlock(Game* game) {
    Block_Copy(game->currentBlock, game->shadowBlock);
    Game_LockBlock(game);
    PlaySound(game->hardDropSound);
}

void Game_MoveBlockRight(Game* game)
{
    if (!game->gameOver) {
        Block_Move(game->currentBlock, (Position) { 0, 1 });
        if (Game_IsBlockOutside(game) || Game_BlockFits(game) == false)
            Block_Move(game->currentBlock, (Position) { 0, -1 });
    }
}

void Game_MoveBlockLeft(Game* game)
{
    if (!game->gameOver) {
        Block_Move(game->currentBlock, (Position) { 0, -1 });
        if (Game_IsBlockOutside(game) || Game_BlockFits(game) == false)
            Block_Move(game->currentBlock, (Position) { 0, 1 });
    }
}

bool isBlockOutside(const Board* board, const Block* block) {
    assert(board && block);
    Position positions[4];
    size_t count;
    Block_GetCellPositions(block, positions, &count);

    for (size_t i = 0; i < count; i++) {
        if (Board_IsCellOutside(board, positions[i].row, positions[i].column)) {
            return true;
        }
    }
    return false;
}

bool Game_IsBlockOutside(const Game* game)
{
    assert(game->currentBlock != NULL);
    return isBlockOutside(game->board, game->currentBlock);
}

void Game_RotateBlock(Game* game)
{
    assert(game->currentBlock != NULL);
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
    assert(game->currentBlock != NULL);
    Position positions[NUM_BLOCK_CELLS];
    size_t count;
    Block_GetCellPositions(game->currentBlock, positions, &count);
    assert(count > 0);

    for (size_t i = 0; i < count; i++) {
        assert(positions[i].row < BOARD_ROWS);
        assert(positions[i].column < BOARD_COLUMNS);
        game->board->grid[positions[i].row][positions[i].column] = game->currentBlock->id;
    }

    Block_Free(game->currentBlock);
    game->currentBlock = game->nextBlock;
    game->currentBlock->rotationState = 0;
    game->nextBlock = GetRandomBlock();

    if (Game_BlockFits(game) == false)
        game->gameOver = true;

    unsigned int rowsCleared = Board_ClearFullRows(game->board);
    if (rowsCleared > 0) {
        PlaySound(game->clearSound);
        Game_UpdateScore(game, rowsCleared, 0);
    } else {
        PlaySound(game->softDropSound);
    }
}

bool blockFits(const Board* board, const Block* block) {
    Position positions[NUM_BLOCK_CELLS];
    size_t count;
    Block_GetCellPositions(block, positions, &count);

    for (size_t i = 0; i < count; i++) {
        if (Board_IsEmpty(board, positions[i].row, positions[i].column) == false) {
            return false;
        }
    }
    return true;
}

bool Game_BlockFits(const Game* game)
{
    assert(game->currentBlock != NULL);
    return blockFits(game->board, game->currentBlock);
}

void Game_UpdateShadowBlock(Game* game) {
    Block_Copy(game->shadowBlock, game->currentBlock);

    while (!isBlockOutside(game->board, game->shadowBlock) && blockFits(game->board, game->shadowBlock))
        Block_Move(game->shadowBlock, (Position) { 1, 0 });

    Block_Move(game->shadowBlock, (Position) { -1, 0 });
}

void Game_Reset(Game* game)
{
    assert(game->board != NULL);
    Board_Reset(game->board);

    // Free current blocks
    Block_Free(game->currentBlock);
    Block_Free(game->nextBlock);
    Block_Free(game->shadowBlock);

    // Create new blocks
    game->currentBlock = GetRandomBlock();
    game->nextBlock = GetRandomBlock();
    game->shadowBlock = GetRandomBlock();
    game->score = 0;
}

void Game_UpdateScore(Game* game, uint32_t linesCleared, uint32_t moveDownPoints)
{
    game->score += moveDownPoints + linesCleared;
}