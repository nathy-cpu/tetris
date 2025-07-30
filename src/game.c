#include "tetris.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

bool EventTriggered(double *lastUpdateTime, double interval)
{
    double currentTime = GetTime();
    if (currentTime - *lastUpdateTime >= interval)
    {
        *lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

Game *Game_Init()
{
    #define blockNum 7
    Game *game = malloc(sizeof(Game));
    assert(game != NULL);
    game->gameOver = false;
    game->score = 0;
    game->numBlocks = blockNum;
    game->grid = Grid_Init();
    // Store pointers to heap-allocated blocks
    const char blockTypes[blockNum] = {'I', 'J', 'L', 'O', 'S', 'T', 'Z'};

    Array blocks = Array_Init(blockNum, sizeof(Block *));
    for (size_t i = 0; i < blockNum; i++)
    {
        Block *block = Block_Init(blockTypes[i]);
        Array_Set(&blocks, i, &block);
    }
    game->blocks = blocks;
    game->currentBlock = Block_Clone(Game_GetRandomBlock(game));
    game->shadowBlock = Block_Clone(Game_GetRandomBlock(game));
    game->currentBlock = Block_Clone(Game_GetRandomBlock(game));

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

void Game_Close(Game *game)
{
    for (size_t i = 0; i < game->numBlocks; i++) {
        Block_Free(Array_Get(&game->blocks, i));
    }
    Array_Free(&game->blocks);
    Block_Free(game->currentBlock);
    Block_Free(game->nextBlock);
    Block_Free(game->shadowBlock);
    Grid_Free(game->grid);
    UnloadSound(game->rotateSound);
    UnloadSound(game->clearSound);
    UnloadSound(game->moveSound);
    UnloadSound(game->hardDropSound);
    UnloadSound(game->softDropSound);
    UnloadMusicStream(game->music);
    UnloadFont(game->font);
    UnloadTexture(game->tileSpriteSheet);
    CloseAudioDevice();
    free(game);
}

void Game_Update(Game *game)
{
    static double dropTimer = 0;
    UpdateMusicStream(game->music);
    Game_HandleInput(game);

    if (EventTriggered(&dropTimer, 0.3))
        Game_MoveBlockDown(game);

    Game_Draw(game);
    EndDrawing();
}

void Game_Draw(const Game *game)
{
    assert(game->grid != NULL);
    assert(game->currentBlock != NULL);
    assert(game->nextBlock != NULL);
    BeginDrawing();
    ClearBackground(darkBlue);
    DrawTextEx(game->font, "Score", (Vector2){365, 15}, 38, 2, WHITE);
    DrawTextEx(game->font, "Next", (Vector2){370, 175}, 38, 2, WHITE);

    if (game->gameOver)
        DrawTextEx(game->font, "GAME OVER", (Vector2){320, 450}, 38, 2, WHITE);

    DrawRectangleRounded((Rectangle){320, 55, 170, 60}, 0.3f, 6, lightBlue);

    char scoreText[10];
    sprintf(scoreText, "%d", game->score);
    Vector2 textSize = MeasureTextEx(game->font, scoreText, 38, 2);

    DrawTextEx(game->font, scoreText, (Vector2){320 + (170 - textSize.x) / 2, 65}, 38, 2, WHITE);
    DrawRectangleRounded((Rectangle){320, 215, 170, 180}, 0.3f, 6, lightBlue);
    Grid_Draw(game->grid);
    Block_Draw(game->currentBlock, 11, 11);

    switch (game->nextBlock->id)
    {
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

void Game_HandleInput(Game *game)
{
    static float moveDelay = 0.0f;
    const float initialDelay = 0.3f;
    const float repeatDelay = 0.07f;
    float frameTime = GetFrameTime();

    int keyPressed = GetKeyPressed();

    if (game->gameOver && keyPressed != 0)
    {
        game->gameOver = false;
        Game_Reset(game);
    }

    switch (keyPressed)
    {
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

    if (IsKeyDown(KEY_LEFT))
    {
        moveDelay += frameTime;
        if (moveDelay >= initialDelay)
        {
            Game_MoveBlockLeft(game);
            moveDelay = initialDelay - repeatDelay;
        }
    }
    else if (IsKeyDown(KEY_RIGHT))
    {
        moveDelay += frameTime;
        if (moveDelay >= initialDelay)
        {
            Game_MoveBlockRight(game);
            moveDelay = initialDelay - repeatDelay;
        }
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        moveDelay += frameTime;
        if (moveDelay >= initialDelay)
        {
            Game_MoveBlockDown(game);
            moveDelay = initialDelay - repeatDelay;
        }
    }
    else
    {
        // Reset the delay if no movement key is pressed
        moveDelay = 0.0f;
    }
}

void Game_MoveBlockDown(Game *game)
{
    if (!game->gameOver)
    {
        Block_Move(game->currentBlock, 1, 0);
        if (Game_IsBlockOutside(game) || Game_BlockFits(game) == false)
        {
            Block_Move(game->currentBlock, -1, 0);
            Game_LockBlock(game);
        }
    }
}

void Game_MoveBlockRight(Game *game)
{
    if (!game->gameOver)
    {
        Block_Move(game->currentBlock, 0, 1);
        if (Game_IsBlockOutside(game) || Game_BlockFits(game) == false)
            Block_Move(game->currentBlock, 0, -1);
    }
}

void Game_MoveBlockLeft(Game *game)
{
    if (!game->gameOver)
    {
        Block_Move(game->currentBlock, 0, -1);
        if (Game_IsBlockOutside(game) || Game_BlockFits(game) == false)
            Block_Move(game->currentBlock, 0, 1);
    }
}

Block *Game_GetRandomBlock(Game *game)
{
    int randomIndex = rand() % Array_Size(&game->blocks);
    Block *templateBlock = *(Block **)Array_Get(&game->blocks, randomIndex);
    return Block_Clone(templateBlock);
}

Array Game_GetAllBlocks(const Game *game)
{
    return game->blocks;
}

bool Game_IsBlockOutside(const Game *game)
{
    assert(game->currentBlock != NULL);
    Array tiles = Block_GetCellPositions(game->currentBlock);
    for (size_t i = 0; i < Array_Size(&tiles); i++)
    {
        Position position = *(Position *)Array_Get(&tiles, i);
        if (Grid_IsCellOutside(game->grid, position.row, position.column))
        {
            Array_Free(&tiles);
            return true;
        }
    }
    Array_Free(&tiles);
    return false;
}

void Game_RotateBlock(Game *game)
{
    assert(game->currentBlock != NULL);
    if (!game->gameOver)
    {
        Block_Rotate(game->currentBlock);

        if (Game_IsBlockOutside(game) || Game_BlockFits(game) == false)
            Block_UndoRotation(game->currentBlock);
        else
            PlaySound(game->rotateSound);
    }
}

void Game_LockBlock(Game *game)
{
    assert(game->currentBlock != NULL);
    Array tiles = Block_GetCellPositions(game->currentBlock);
    assert(Array_Size(&tiles) > 0);

    for (size_t i = 0; i < Array_Size(&tiles); i++)
    {
        Position position = *(Position *)Array_Get(&tiles, i);
        assert(position.row >= 0 && position.row < GRID_ROWS);
        assert(position.column >= 0 && position.column < GRID_COLUMNS);
        game->grid->grid[position.row][position.column] = game->currentBlock->id;
    }

    // Move nextBlock to currentBlock (already a clone)
    game->currentBlock = game->nextBlock;
    // Only reset rotationState if needed
    game->currentBlock->rotationState = 0;

    if (Game_BlockFits(game) == false)
        game->gameOver = true;

    // Pick a new nextBlock (clone from template)
    int randomIndex = rand() % Array_Size(&game->blocks);
    Block *templateBlock = *(Block **)Array_Get(&game->blocks, randomIndex);
    game->nextBlock = Block_Clone(templateBlock);

    unsigned int rowsCleared = Grid_ClearFullRows(game->grid);
    if (rowsCleared > 0)
    {
        PlaySound(game->clearSound);
        Game_UpdateScore(game, rowsCleared, 0);
    }
    Array_Free(&tiles);
}

bool Game_BlockFits(const Game *game)
{
    assert(game->currentBlock != NULL);
    Array tiles = Block_GetCellPositions(game->currentBlock);

    for (size_t i = 0; i < Array_Size(&tiles); i++)
    {
        Position position = *(Position *)Array_Get(&tiles, i);
        if (Grid_IsEmpty(game->grid, position.row, position.column) == false)
        {
            Array_Free(&tiles);
            return false;
        }
    }

    Array_Free(&tiles);
    return true;
}

void Game_Reset(Game *game)
{
    assert(game->grid != NULL);
    Grid_Reset(game->grid);
    // Re-initialize grid and blocks
    game->grid = Grid_Init();
    // Recreate block templates
    #define blockNum 7
    const char blockTypes[blockNum] = {'I', 'J', 'L', 'O', 'S', 'T', 'Z'};
    Array blocks = Array_Init(blockNum, sizeof(Block *));
    for (size_t i = 0; i < blockNum; i++)
    {
        Block *block = Block_Init(blockTypes[i]);
        Array_Set(&blocks, i, &block);
    }
    game->blocks = blocks;
    // Spawn new falling blocks (clones)
    int idx1 = rand() % Array_Size(&game->blocks);
    int idx2 = rand() % Array_Size(&game->blocks);
    int idx3 = rand() % Array_Size(&game->blocks);
    game->currentBlock = Block_Clone(*(Block **)Array_Get(&game->blocks, idx1));
    game->shadowBlock = Block_Clone(*(Block **)Array_Get(&game->blocks, idx2));
    game->nextBlock = Block_Clone(*(Block **)Array_Get(&game->blocks, idx3));
    game->score = 0;
}

void Game_UpdateScore(Game *game, uint32_t linesCleared, uint32_t moveDownPoints)
{
    game->score += moveDownPoints + linesCleared;
}