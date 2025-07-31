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

void* AudioThread(void* arg)
{
    Game* game = (Game*)arg;
    while (game->audioThreadRunning) {
        Mutex_Lock(&game->audioMutex);
        if (!game->audioThreadRunning) {
            Mutex_Unlock(&game->audioMutex);
        }

        if (game->gameOver) {
            if (IsMusicStreamPlaying(game->music))
                PauseMusicStream(game->music);
        } else {
            if (!IsMusicStreamPlaying(game->music))
                PlayMusicStream(game->music);

            UpdateMusicStream(game->music);
        }

        Mutex_Unlock(&game->audioMutex);
        SleepFor(50); // 50ms
    }

    return 0;
}

void Game_StartAudioThread(Game* game)
{
    if (game->audioThreadRunning)
        return;
    game->audioThreadRunning = true;
    Mutex_Init(&game->audioMutex);
    Thread_Init(&game->audioThread, AudioThread, game);
}

void Game_StopAudioThread(Game* game)
{
    game->audioThreadRunning = false;
    Thread_Join(&game->audioThread);
    Mutex_Destroy(&game->audioMutex);
}

Game* Game_Init()
{
    Game* game = malloc(sizeof(Game));
    assert(game != NULL);
    game->gameOver = false;
    game->score = 0;
    game->numBlocks = NUM_BLOCKS;
    game->grid = Grid_Init();

    // Initialize block templates
    for (size_t i = 0; i < game->numBlocks; i++) {
        const char blockTypes[NUM_BLOCKS] = { 'I', 'J', 'L', 'O', 'S', 'T', 'Z' };
        game->blocks[i] = Block_Init(blockTypes[i]);
        assert(game->blocks[i] != NULL);
    }

    // Spawn initial blocks
    game->currentBlock = Block_Clone(game->blocks[GetRandomValue(0, INT32_MAX) % game->numBlocks]);
    game->nextBlock = Block_Clone(game->blocks[GetRandomValue(0, INT32_MAX) % game->numBlocks]);
    game->shadowBlock = Block_Clone(game->blocks[GetRandomValue(0, INT32_MAX) % game->numBlocks]);

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

    // Initialize audio thread
    // game->audioThreadRunning = false;
    // Mutex_Init(&game->audioMutex);
    // Game_StartAudioThread(game);

    return game;
}

void Game_Close(Game* game)
{
    // // signal to stop audio thread
    // Mutex_Lock(&game->audioMutex);
    // game->audioThreadRunning = false;
    // Mutex_Unlock(&game->audioMutex);
    //
    // // wait for audio thread to finish
    // Thread_Join(&game->audioThread);
    // Mutex_Destroy(&game->audioMutex);

    for (size_t i = 0; i < game->numBlocks; i++) {
        Block_Free(game->blocks[i]);
    }

    Block_Free(game->currentBlock);
    Block_Free(game->nextBlock);
    Block_Free(game->shadowBlock);
    Grid_Free(game->grid);

    StopMusicStream(game->music);
    UnloadMusicStream(game->music);
    UnloadSound(game->rotateSound);
    UnloadSound(game->clearSound);
    UnloadSound(game->moveSound);
    UnloadSound(game->hardDropSound);
    UnloadSound(game->softDropSound);

    UnloadFont(game->font);
    UnloadTexture(game->tileSpriteSheet);
    CloseAudioDevice();
    free(game);
}

void Game_Update(Game* game)
{
    static double dropTimer = 0;
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
    assert(game->grid != NULL);
    assert(game->currentBlock != NULL);
    assert(game->nextBlock != NULL);
    BeginDrawing();
    ClearBackground(darkBlue);
    DrawTextEx(game->font, "Score", (Vector2) { 365, 15 }, 38, 2, WHITE);
    DrawTextEx(game->font, "Next", (Vector2) { 370, 175 }, 38, 2, WHITE);

    if (game->gameOver) {
        DrawTextEx(game->font, "GAME OVER", (Vector2) { 320, 450 }, 38, 2, WHITE);
    }

    DrawRectangleRounded((Rectangle) { 320, 55, 170, 60 }, 0.3f, 6, lightBlue);

    char scoreText[10];
    sprintf(scoreText, "%d", game->score);
    const Vector2 textSize = MeasureTextEx(game->font, scoreText, 38, 2);

    DrawTextEx(game->font, scoreText, (Vector2) { 320 + (170 - textSize.x) / 2, 65 }, 38, 2, WHITE);
    DrawRectangleRounded((Rectangle) { 320, 215, 170, 180 }, 0.3f, 6, lightBlue);
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
    EndDrawing();
}

void Game_HandleInput(Game* game)
{
    static float moveDelay = 0.0f;
    const float initialDelay = 0.3f;
    const float repeatDelay = 0.07f;
    const float frameTime = GetFrameTime();

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
        Game_MoveBlockDown(game);
        break;
    case KEY_UP:
        Game_RotateBlock(game);
        break;
    default:
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

Block* Game_GetRandomBlock(const Game* game)
{
    int randomIndex = GetRandomValue(0, INT32_MAX) % NUM_BLOCKS;
    return Block_Clone(game->blocks[randomIndex]);
}

bool Game_IsBlockOutside(const Game* game)
{
    assert(game->currentBlock != NULL);
    Position positions[4];
    size_t count;
    Block_GetCellPositions(game->currentBlock, positions, &count);

    for (size_t i = 0; i < count; i++) {
        if (Grid_IsCellOutside(game->grid, positions[i].row, positions[i].column)) {
            return true;
        }
    }
    return false;
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
        assert(positions[i].row < GRID_ROWS);
        assert(positions[i].column < GRID_COLUMNS);
        game->grid->grid[positions[i].row][positions[i].column] = game->currentBlock->id;
    }

    // Free the old current block
    Block_Free(game->currentBlock);

    // Move nextBlock to currentBlock
    game->currentBlock = game->nextBlock;
    game->currentBlock->rotationState = 0;

    if (Game_BlockFits(game) == false)
        game->gameOver = true;

    // Create new nextBlock
    game->nextBlock = Game_GetRandomBlock(game);

    unsigned int rowsCleared = Grid_ClearFullRows(game->grid);
    if (rowsCleared > 0) {
        PlaySound(game->clearSound);
        Game_UpdateScore(game, rowsCleared, 0);
    }
}

bool Game_BlockFits(const Game* game)
{
    assert(game->currentBlock != NULL);
    Position positions[NUM_BLOCK_CELLS];
    size_t count;
    Block_GetCellPositions(game->currentBlock, positions, &count);

    for (size_t i = 0; i < count; i++) {
        if (Grid_IsEmpty(game->grid, positions[i].row, positions[i].column) == false) {
            return false;
        }
    }
    return true;
}

void Game_Reset(Game* game)
{
    assert(game->grid != NULL);
    Grid_Reset(game->grid);

    // Free current blocks
    Block_Free(game->currentBlock);
    Block_Free(game->nextBlock);
    Block_Free(game->shadowBlock);

    // Create new blocks
    game->currentBlock = Game_GetRandomBlock(game);
    game->nextBlock = Game_GetRandomBlock(game);
    game->shadowBlock = Game_GetRandomBlock(game);
    game->score = 0;
}

void Game_UpdateScore(Game* game, uint32_t linesCleared, uint32_t moveDownPoints)
{
    game->score += moveDownPoints + linesCleared;
}