#include "tetris.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Block_GetCellPositions(const Block* block, Position* positions, size_t* count)
{
    *count = NUM_BLOCK_CELLS;
    for (int i = 0; i < NUM_BLOCK_CELLS; i++) {
        positions[i].row = BLOCK_LAYOUTS[block->id - 1][block->rotationState][i].row + block->rowOffset;
        positions[i].column = BLOCK_LAYOUTS[block->id - 1][block->rotationState][i].column + block->columnOffset;
    }
}

void Block_Draw(const Block* block, int offsetX, int offsetY, Texture2D tileSpriteSheet, float opacity)
{
    Position positions[NUM_BLOCK_CELLS];
    size_t count;
    Block_GetCellPositions(block, positions, &count);

    for (size_t i = 0; i < count; i++) {
        DrawTexturePro(
            tileSpriteSheet,
            (Rectangle) { (block->id - 1) * 16, 0, 16, 16 },
            (Rectangle) { positions[i].column * CELL_SIZE + offsetX, positions[i].row * CELL_SIZE + offsetY, CELL_SIZE - 1, CELL_SIZE - 1 },
            (Vector2) { 0, 0 }, 0, Fade(WHITE, opacity));
    }
}

void Block_Move(Block* block, Position position)
{
    block->rowOffset += position.row;
    block->columnOffset += position.column;
}

void Block_Rotate(Block* block)
{
    block->rotationState++;
    if (block->rotationState >= block->numRotations)
        block->rotationState = 0;
}

void Block_UndoRotation(Block* block)
{
    if (block->rotationState == 0)
        block->rotationState = block->numRotations - 1;
    else
        block->rotationState--;
}

Block* Block_Init(enum BlockType type)
{
    Block* block = malloc(sizeof(Block));
    // CELL_SIZE = 30;
    block->rotationState = 0;
    block->rowOffset = 0;
    block->columnOffset = 0;
    block->id = (uint8_t)type;
    block->numRotations = BLOCK_ROTAIONS[type - 1];
    Block_Move(block, BLOCK_OFFSETS[type - 1]);

    return block;
}

Block* Block_Clone(const Block* src)
{
    Block* block = malloc(sizeof(Block));
    memcpy(block, src, sizeof(Block));
    return block;
}

void Block_Free(Block* block)
{
    if (block) {
        free(block);
    }
}