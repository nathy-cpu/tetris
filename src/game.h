#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <stdint.h>

#include "arena.h"
#include "block.h"
#include "grid.h"

typedef struct
{
    Music music;
    Font font;
    Sound rotateSound;
    Sound clearSound;
    Sound moveSound;
    Sound softDropSound;
    Sound hardDropSound;
    Array blocks;
    Texture2D tileSpriteSheet;
    Grid* grid;
    Block* currentBlock;
    Block* nextBlock;
    Block* shadowBlock;
    Arena* arena;
    uint32_t score;
    bool gameOver;

} Game;

Game* Game_Init();

void Game_Update(Game* game);

void Game_Close(Game* game);

void Game_Draw(const Game* game);

void Game_HandleInput(Game* game);

void Game_MoveBlockDown(Game* game);

void Game_MoveBlockRight(Game* game);

void Game_MoveBlockLeft(Game* game);

Block* Game_GetRandomBlock(Game* game);

Array Game_GetAllBlocks(Game* game);

bool Game_IsBlockOutside(Game* game);

void Game_RotateBlock(Game* game);

void Game_LockBlock(Game* game);

bool Game_BlockFits(Game* game);

void Game_Reset(Game* game);

void Game_UpdateScore(Game* game, uint32_t linesCleared, uint32_t moveDownPoints);

#endif
