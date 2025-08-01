#ifndef TETRIS_H
#define TETRIS_H

#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Position
typedef struct
{
    int8_t row;
    int8_t column;

} Position;

// Block

enum BlockType : uint8_t {
    Z = 1,
    S,
    T,
    L,
    J,
    I,
    O
};

#define ROTATION_STATES 4
#define NUM_BLOCK_CELLS 4
#define NUM_COLORS 8
#define CELL_SIZE 30

typedef struct
{
    uint8_t id;
    int8_t rotationState;
    uint8_t rowOffset;
    uint8_t columnOffset;
    uint8_t numRotations;

} Block;

void Block_GetCellPositions(const Block* block, Position* positions, size_t* count);

void Block_Draw(const Block* block, int offsetX, int offsetY, Texture2D tileSpriteSheet, float opacity);

void Block_Move(Block* block, Position position);

void Block_Rotate(Block* block);

void Block_UndoRotation(Block* block);

Block* Block_Init(enum BlockType type);

Block* Block_Clone(const Block* src);

void Block_Free(Block* block);

Block* GetRandomBlock();

// Colors
static const Color darkGrey = (Color) { 26, 31, 40, 255 };
static const Color lightBlue = (Color) { 59, 85, 162, 255 };
static const Color darkBlue = (Color) { 44, 44, 127, 255 };
static const Color blue = (Color) { 13, 64, 216, 255 };
static const Color orange = (Color) { 226, 116, 17, 255 };
static const Color cyan = (Color) { 21, 204, 209, 255 };
static const Color yellow = (Color) { 237, 234, 4, 255 };
static const Color green = (Color) { 47, 230, 23, 255 };
static const Color purple = (Color) { 166, 0, 247, 255 };
static const Color red = (Color) { 232, 18, 18, 255 };

// Board

#define BOARD_ROWS 20
#define BOARD_COLUMNS 10
#define BOARD_CELL_SIZE 30
#define BOARD_PADDING 11

typedef struct
{
    uint8_t numRows;
    uint8_t numCols;
    uint8_t grid[BOARD_ROWS][BOARD_COLUMNS];

} Board;

Board* Board_Init();

void Board_Free(Board* board);

void Board_Reset(Board* board);

void Board_Print(const Board* board);

void Board_Draw(const Board* board, Texture2D tileSpriteSheet);

bool Board_IsCellOutside(const Board* board, int8_t row, int8_t column);

bool Board_IsEmpty(const Board* board, uint8_t row, uint8_t column);

bool Board_IsRowFull(const Board* board, uint8_t row);

void Board_ClearRow(Board* board, uint8_t row);

void Board_MoveRowDown(Board* board, uint8_t row, uint8_t numRows);

uint8_t Board_ClearFullRows(Board* board);

// Game
#define NUM_BLOCKS 7

typedef struct
{
    Music music;
    Font font;
    Sound rotateSound;
    Sound clearSound;
    Sound moveSound;
    Sound softDropSound;
    Sound hardDropSound;
    size_t numBlocks;
    Texture2D tileSpriteSheet;
    Board* board;
    Block* currentBlock;
    Block* nextBlock;
    Block* shadowBlock;
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

bool Game_IsBlockOutside(const Game* game);

void Game_RotateBlock(Game* game);

void Game_LockBlock(Game* game);

bool Game_BlockFits(const Game* game);

void Game_Reset(Game* game);

void Game_UpdateScore(Game* game, uint32_t linesCleared, uint32_t moveDownPoints);

static const Position BLOCK_LAYOUTS[NUM_BLOCKS][ROTATION_STATES][NUM_BLOCK_CELLS] = {
    { // Z
        {
            (Position) { 0, 0 },
            (Position) { 0, 1 },
            (Position) { 1, 1 },
            (Position) { 1, 2 },
        },
        {
            (Position) { 0, 2 },
            (Position) { 1, 1 },
            (Position) { 1, 2 },
            (Position) { 2, 1 },
        },
        {
            (Position) { 1, 0 },
            (Position) { 1, 1 },
            (Position) { 2, 1 },
            (Position) { 2, 2 },
        },
        {
            (Position) { 0, 1 },
            (Position) { 1, 0 },
            (Position) { 1, 1 },
            (Position) { 2, 0 },
        } },
    { // S
        {
            (Position) { 0, 1 },
            (Position) { 0, 2 },
            (Position) { 1, 0 },
            (Position) { 1, 1 },
        },
        {
            (Position) { 0, 1 },
            (Position) { 1, 1 },
            (Position) { 1, 2 },
            (Position) { 2, 2 },
        },
        {
            (Position) { 1, 1 },
            (Position) { 1, 2 },
            (Position) { 2, 0 },
            (Position) { 2, 1 },
        },
        {
            (Position) { 0, 0 },
            (Position) { 1, 0 },
            (Position) { 1, 1 },
            (Position) { 2, 1 },
        } },
    { // T
        {
            (Position) { 0, 1 },
            (Position) { 1, 0 },
            (Position) { 1, 1 },
            (Position) { 1, 2 },
        },
        {
            (Position) { 0, 1 },
            (Position) { 1, 1 },
            (Position) { 1, 2 },
            (Position) { 2, 1 },
        },
        {
            (Position) { 1, 0 },
            (Position) { 1, 1 },
            (Position) { 1, 2 },
            (Position) { 2, 1 },
        },
        {
            (Position) { 0, 1 },
            (Position) { 1, 0 },
            (Position) { 1, 1 },
            (Position) { 2, 1 },
        } },
    { // L
        {
            (Position) { 0, 2 },
            (Position) { 1, 0 },
            (Position) { 1, 1 },
            (Position) { 1, 2 } },
        { (Position) { 0, 1 },
            (Position) { 1, 1 },
            (Position) { 2, 1 },
            (Position) { 2, 2 } },
        { (Position) { 1, 0 },
            (Position) { 1, 1 },
            (Position) { 1, 2 },
            (Position) { 2, 0 } },
        { (Position) { 0, 0 },
            (Position) { 0, 1 },
            (Position) { 1, 1 },
            (Position) { 2, 1 } } },
    { // J
        {
            (Position) { 0, 0 },
            (Position) { 1, 0 },
            (Position) { 1, 1 },
            (Position) { 1, 2 },
        },
        {
            (Position) { 0, 1 },
            (Position) { 0, 2 },
            (Position) { 1, 1 },
            (Position) { 2, 1 },
        },
        {
            (Position) { 1, 0 },
            (Position) { 1, 1 },
            (Position) { 1, 2 },
            (Position) { 2, 2 },
        },
        {
            (Position) { 0, 1 },
            (Position) { 1, 1 },
            (Position) { 2, 0 },
            (Position) { 2, 1 },
        } },
    { // I
        {
            (Position) { 1, 0 },
            (Position) { 1, 1 },
            (Position) { 1, 2 },
            (Position) { 1, 3 },
        },
        {
            (Position) { 0, 2 },
            (Position) { 1, 2 },
            (Position) { 2, 2 },
            (Position) { 3, 2 },
        },
        {
            (Position) { 2, 0 },
            (Position) { 2, 1 },
            (Position) { 2, 2 },
            (Position) { 2, 3 },
        },
        {
            (Position) { 0, 1 },
            (Position) { 1, 1 },
            (Position) { 2, 1 },
            (Position) { 3, 1 },
        } },
    { // O
        {
            (Position) { 0, 0 },
            (Position) { 0, 1 },
            (Position) { 1, 0 },
            (Position) { 1, 1 },
        } },
};

static const Position BLOCK_OFFSETS[NUM_BLOCKS] = {
    (Position) { 0, 3 }, // Z
    (Position) { 0, 3 }, // S
    (Position) { 0, 3 }, // T
    (Position) { 0, 3 }, // L
    (Position) { 0, 3 }, // J
    (Position) { -1, 3 }, // I
    (Position) { 0, 4 }, // O
};

static const uint8_t BLOCK_ROTAIONS[NUM_BLOCKS] = {
    4, 4, 4, 4, 4, 4, 1
};

// Some constants

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 620
#define SCREEN_TITLE "Tetris"
#define FONT_SIZE 38
#define FONT_SPACING 2
#define SPRITE_SIZE 16

#endif // TETRIS_H
