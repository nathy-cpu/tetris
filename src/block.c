#include "tetris.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Block_GetCellPositions(const Block* block, Position* positions, size_t* count)
{
    *count = NUM_BLOCK_CELLS;
    for (int i = 0; i < NUM_BLOCK_CELLS; i++) {
        positions[i].row = block->cells[block->rotationState][i].row + block->rowOffset;
        positions[i].column = block->cells[block->rotationState][i].column + block->columnOffset;
    }
}

void Block_Draw(const Block* block, int offsetX, int offsetY)
{
    Position positions[NUM_BLOCK_CELLS];
    size_t count;
    Block_GetCellPositions(block, positions, &count);

    for (size_t i = 0; i < count; i++) {
        Color color = block->colors[block->id];
        DrawRectangle(positions[i].column * block->cellSize + offsetX,
            positions[i].row * block->cellSize + offsetY,
            block->cellSize - 1, block->cellSize - 1, color);
    }
}

void Block_Move(Block* block, int rows, int columns)
{
    block->rowOffset += rows;
    block->columnOffset += columns;
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

Block* Block_Init(char type)
{
    Block* block = malloc(sizeof(Block));
    block->cellSize = 30;
    block->rotationState = 0;
    block->rowOffset = 0;
    block->columnOffset = 0;

    // Initialize colors
    block->colors[0] = darkGrey;
    block->colors[1] = blue;
    block->colors[2] = orange;
    block->colors[3] = cyan;
    block->colors[4] = yellow;
    block->colors[5] = green;
    block->colors[6] = purple;
    block->colors[7] = red;

    switch (type) {
    case 'L':
        block->id = 1;
        block->numRotations = 4;
        // Rotation 0
        block->cells[0][0] = (Position) { 0, 2 };
        block->cells[0][1] = (Position) { 1, 0 };
        block->cells[0][2] = (Position) { 1, 1 };
        block->cells[0][3] = (Position) { 1, 2 };
        // Rotation 1
        block->cells[1][0] = (Position) { 0, 1 };
        block->cells[1][1] = (Position) { 1, 1 };
        block->cells[1][2] = (Position) { 2, 1 };
        block->cells[1][3] = (Position) { 2, 2 };
        // Rotation 2
        block->cells[2][0] = (Position) { 1, 0 };
        block->cells[2][1] = (Position) { 1, 1 };
        block->cells[2][2] = (Position) { 1, 2 };
        block->cells[2][3] = (Position) { 2, 0 };
        // Rotation 3
        block->cells[3][0] = (Position) { 0, 0 };
        block->cells[3][1] = (Position) { 0, 1 };
        block->cells[3][2] = (Position) { 1, 1 };
        block->cells[3][3] = (Position) { 2, 1 };
        Block_Move(block, 0, 3);
        break;

    case 'J':
        block->id = 2;
        block->numRotations = 4;
        // Rotation 0
        block->cells[0][0] = (Position) { 0, 0 };
        block->cells[0][1] = (Position) { 1, 0 };
        block->cells[0][2] = (Position) { 1, 1 };
        block->cells[0][3] = (Position) { 1, 2 };
        // Rotation 1
        block->cells[1][0] = (Position) { 0, 1 };
        block->cells[1][1] = (Position) { 0, 2 };
        block->cells[1][2] = (Position) { 1, 1 };
        block->cells[1][3] = (Position) { 2, 1 };
        // Rotation 2
        block->cells[2][0] = (Position) { 1, 0 };
        block->cells[2][1] = (Position) { 1, 1 };
        block->cells[2][2] = (Position) { 1, 2 };
        block->cells[2][3] = (Position) { 2, 2 };
        // Rotation 3
        block->cells[3][0] = (Position) { 0, 1 };
        block->cells[3][1] = (Position) { 1, 1 };
        block->cells[3][2] = (Position) { 2, 0 };
        block->cells[3][3] = (Position) { 2, 1 };
        Block_Move(block, 0, 3);
        break;

    case 'I':
        block->id = 3;
        block->numRotations = 4;
        // Rotation 0
        block->cells[0][0] = (Position) { 1, 0 };
        block->cells[0][1] = (Position) { 1, 1 };
        block->cells[0][2] = (Position) { 1, 2 };
        block->cells[0][3] = (Position) { 1, 3 };
        // Rotation 1
        block->cells[1][0] = (Position) { 0, 2 };
        block->cells[1][1] = (Position) { 1, 2 };
        block->cells[1][2] = (Position) { 2, 2 };
        block->cells[1][3] = (Position) { 3, 2 };
        // Rotation 2
        block->cells[2][0] = (Position) { 2, 0 };
        block->cells[2][1] = (Position) { 2, 1 };
        block->cells[2][2] = (Position) { 2, 2 };
        block->cells[2][3] = (Position) { 2, 3 };
        // Rotation 3
        block->cells[3][0] = (Position) { 0, 1 };
        block->cells[3][1] = (Position) { 1, 1 };
        block->cells[3][2] = (Position) { 2, 1 };
        block->cells[3][3] = (Position) { 3, 1 };
        Block_Move(block, -1, 3);
        break;

    case 'O':
        block->id = 4;
        block->numRotations = 1;
        // Only one rotation for O block
        block->cells[0][0] = (Position) { 0, 0 };
        block->cells[0][1] = (Position) { 0, 1 };
        block->cells[0][2] = (Position) { 1, 0 };
        block->cells[0][3] = (Position) { 1, 1 };
        Block_Move(block, 0, 4);
        break;

    case 'S':
        block->id = 5;
        block->numRotations = 4;
        // Rotation 0
        block->cells[0][0] = (Position) { 0, 1 };
        block->cells[0][1] = (Position) { 0, 2 };
        block->cells[0][2] = (Position) { 1, 0 };
        block->cells[0][3] = (Position) { 1, 1 };
        // Rotation 1
        block->cells[1][0] = (Position) { 0, 1 };
        block->cells[1][1] = (Position) { 1, 1 };
        block->cells[1][2] = (Position) { 1, 2 };
        block->cells[1][3] = (Position) { 2, 2 };
        // Rotation 2
        block->cells[2][0] = (Position) { 1, 1 };
        block->cells[2][1] = (Position) { 1, 2 };
        block->cells[2][2] = (Position) { 2, 0 };
        block->cells[2][3] = (Position) { 2, 1 };
        // Rotation 3
        block->cells[3][0] = (Position) { 0, 0 };
        block->cells[3][1] = (Position) { 1, 0 };
        block->cells[3][2] = (Position) { 1, 1 };
        block->cells[3][3] = (Position) { 2, 1 };
        Block_Move(block, 0, 3);
        break;

    case 'T':
        block->id = 6;
        block->numRotations = 4;
        // Rotation 0
        block->cells[0][0] = (Position) { 0, 1 };
        block->cells[0][1] = (Position) { 1, 0 };
        block->cells[0][2] = (Position) { 1, 1 };
        block->cells[0][3] = (Position) { 1, 2 };
        // Rotation 1
        block->cells[1][0] = (Position) { 0, 1 };
        block->cells[1][1] = (Position) { 1, 1 };
        block->cells[1][2] = (Position) { 1, 2 };
        block->cells[1][3] = (Position) { 2, 1 };
        // Rotation 2
        block->cells[2][0] = (Position) { 1, 0 };
        block->cells[2][1] = (Position) { 1, 1 };
        block->cells[2][2] = (Position) { 1, 2 };
        block->cells[2][3] = (Position) { 2, 1 };
        // Rotation 3
        block->cells[3][0] = (Position) { 0, 1 };
        block->cells[3][1] = (Position) { 1, 0 };
        block->cells[3][2] = (Position) { 1, 1 };
        block->cells[3][3] = (Position) { 2, 1 };
        Block_Move(block, 0, 3);
        break;

    case 'Z':
        block->id = 7;
        block->numRotations = 4;
        // Rotation 0
        block->cells[0][0] = (Position) { 0, 0 };
        block->cells[0][1] = (Position) { 0, 1 };
        block->cells[0][2] = (Position) { 1, 1 };
        block->cells[0][3] = (Position) { 1, 2 };
        // Rotation 1
        block->cells[1][0] = (Position) { 0, 2 };
        block->cells[1][1] = (Position) { 1, 1 };
        block->cells[1][2] = (Position) { 1, 2 };
        block->cells[1][3] = (Position) { 2, 1 };
        // Rotation 2
        block->cells[2][0] = (Position) { 1, 0 };
        block->cells[2][1] = (Position) { 1, 1 };
        block->cells[2][2] = (Position) { 2, 1 };
        block->cells[2][3] = (Position) { 2, 2 };
        // Rotation 3
        block->cells[3][0] = (Position) { 0, 1 };
        block->cells[3][1] = (Position) { 1, 0 };
        block->cells[3][2] = (Position) { 1, 1 };
        block->cells[3][3] = (Position) { 2, 0 };
        Block_Move(block, 0, 3);
        break;

    default:
        fprintf(stderr, "Unknown block type: %c\n", type);
        free(block);
        return NULL;
    }

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