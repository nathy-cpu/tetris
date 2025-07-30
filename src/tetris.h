#ifndef TETRIS_H
#define TETRIS_H

#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Arena
#define GB1 (1024 * 1024 * 1024)
#define MB512 (1024 * 1024 * 1024) / 2
#define MB256 (1024 * 1024 * 1024) / 4
#define MB128 (1024 * 1024 * 1024) / 8
#define MB64 (1024 * 1024 * 1024) / 16
#define MB32 (1024 * 1024 * 1024) / 32

typedef struct {
    size_t size;
    size_t head;
    size_t tail;
    uint8_t* memory;
} Arena;

Arena* Arena_Init(size_t size);

void* Arena_Allocate(Arena* arena, size_t size);

void* Arena_AllocateAligned(Arena* arena, size_t size);

void Arena_Pop(Arena* arena);

void Arena_Reset(Arena* arena);

void Arena_Free(Arena* arena);

// Array
typedef struct
{
    size_t size;
    size_t elementSize;
    char* data;
} Array;

#define ARRAY_EMPTY (Array) { .data = NULL, .size = 0, .elementSize = 0 }

Array Array_Init(Arena* arena, size_t size, size_t elementSize);

void Array_Set(Array* array, size_t index, const void* element);

void* Array_Get(const Array* array, size_t index);

size_t Array_Size(const Array* array);

Array Array_Malloc(size_t size, size_t elementSize);

void Array_Free(Array* array);

// Block
typedef struct
{
    Array cells;
    Array colors;
    uint8_t id;
    uint8_t cellSize;
    int8_t rotationState;
    uint8_t rowOffset;
    uint8_t columnOffset;

} Block;

Array Block_GetCellPositions(const Block* block);

void Block_Draw(const Block* block, int offsetX, int offsetY);

void Block_Move(Block* block, int rows, int columns);

void Block_Rotate(Block* block);

void Block_UndoRotation(Block* block);

Block* Block_Init(Arena* arena, char type);

// Colors
static const Color darkGrey = (Color) { 26, 31, 40, 255 };
static const Color green = (Color) { 47, 230, 23, 255 };
static const Color red = (Color) { 232, 18, 18, 255 };
static const Color orange = (Color) { 226, 116, 17, 255 };
static const Color yellow = (Color) { 237, 234, 4, 255 };
static const Color purple = (Color) { 166, 0, 247, 255 };
static const Color cyan = (Color) { 21, 204, 209, 255 };
static const Color blue = (Color) { 13, 64, 216, 255 };
static const Color lightBlue = (Color) { 59, 85, 162, 255 };
static const Color darkBlue = (Color) { 44, 44, 127, 255 };

Array GetCellColors(Arena* arena);

// Grid
typedef struct
{
    Array colors;
    int cellSize;
    uint8_t numRows;
    uint8_t numCols;
    uint8_t grid[20][10];

} Grid;

Grid* Grid_Init(Arena* arena);

void Grid_Reset(Grid* grid);

Color Grid_GetCellColor(const Grid* grid, uint8_t cellValue);

void Grid_Print(const Grid* grid);

void Grid_Draw(const Grid* grid);

bool Grid_IsCellOutside(const Grid* grid, int8_t row, int8_t column);

bool Grid_IsEmpty(const Grid* grid, uint8_t row, uint8_t column);

bool Grid_IsRowFull(const Grid* grid, uint8_t row);

void Grid_ClearRow(Grid* grid, uint8_t row);

void Grid_MoveRowDown(Grid* grid, uint8_t row, uint8_t numRows);

uint8_t Grid_ClearFullRows(Grid* grid);

// Position
typedef struct
{
    uint8_t row;
    uint8_t column;

} Position;

Position Position_Init(uint8_t row, uint8_t column);

// Game
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

Array Game_GetAllBlocks(const Game* game);

bool Game_IsBlockOutside(const Game* game);

void Game_RotateBlock(Game* game);

void Game_LockBlock(Game* game);

bool Game_BlockFits(const Game* game);

void Game_Reset(Game* game);

void Game_UpdateScore(Game* game, uint32_t linesCleared, uint32_t moveDownPoints);

#endif // TETRIS_H