#ifndef BLOCK_H
#define BLOCK_H

#include <raylib.h>
#include <stdint.h>

#include "array.h"
#include "position.h"

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

#endif
