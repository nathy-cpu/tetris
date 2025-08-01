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

    // Initialize colors
    // block->colors[0] = darkGrey;
    // block->colors[1] = blue;
    // block->colors[2] = orange;
    // block->colors[3] = cyan;
    // block->colors[4] = yellow;
    // block->colors[5] = green;
    // block->colors[6] = purple;
    // block->colors[7] = red;

    // switch (type) {
    // case L:
    //     block->id = 1;
    //     block->numRotations = 4;
    //     // Rotation 0
    //     BLOCK_LAYOUTS[block->id - 1][0][0] = (Position) { 0, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][0][1] = (Position) { 1, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][0][2] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][0][3] = (Position) { 1, 2 };
    //     // Rotation 1
    //     BLOCK_LAYOUTS[block->id - 1][1][0] = (Position) { 0, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][1][1] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][1][2] = (Position) { 2, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][1][3] = (Position) { 2, 2 };
    //     // Rotation 2
    //     BLOCK_LAYOUTS[block->id - 1][2][0] = (Position) { 1, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][2][1] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][2][2] = (Position) { 1, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][2][3] = (Position) { 2, 0 };
    //     // Rotation 3
    //     BLOCK_LAYOUTS[block->id - 1][3][0] = (Position) { 0, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][3][1] = (Position) { 0, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][3][2] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][3][3] = (Position) { 2, 1 };
    //     Block_Move(block, 0, 3);
    //     break;
    //
    // case J:
    //     block->id = 2;
    //     block->numRotations = 4;
    //     // Rotation 0
    //     BLOCK_LAYOUTS[block->id - 1][0][0] = (Position) { 0, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][0][1] = (Position) { 1, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][0][2] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][0][3] = (Position) { 1, 2 };
    //     // Rotation 1
    //     BLOCK_LAYOUTS[block->id - 1][1][0] = (Position) { 0, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][1][1] = (Position) { 0, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][1][2] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][1][3] = (Position) { 2, 1 };
    //     // Rotation 2
    //     BLOCK_LAYOUTS[block->id - 1][2][0] = (Position) { 1, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][2][1] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][2][2] = (Position) { 1, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][2][3] = (Position) { 2, 2 };
    //     // Rotation 3
    //     BLOCK_LAYOUTS[block->id - 1][3][0] = (Position) { 0, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][3][1] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][3][2] = (Position) { 2, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][3][3] = (Position) { 2, 1 };
    //     Block_Move(block, 0, 3);
    //     break;
    //
    // case I:
    //     block->id = 3;
    //     block->numRotations = 4;
    //     // Rotation 0
    //     BLOCK_LAYOUTS[block->id - 1][0][0] = (Position) { 1, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][0][1] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][0][2] = (Position) { 1, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][0][3] = (Position) { 1, 3 };
    //     // Rotation 1
    //     BLOCK_LAYOUTS[block->id - 1][1][0] = (Position) { 0, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][1][1] = (Position) { 1, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][1][2] = (Position) { 2, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][1][3] = (Position) { 3, 2 };
    //     // Rotation 2
    //     BLOCK_LAYOUTS[block->id - 1][2][0] = (Position) { 2, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][2][1] = (Position) { 2, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][2][2] = (Position) { 2, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][2][3] = (Position) { 2, 3 };
    //     // Rotation 3
    //     BLOCK_LAYOUTS[block->id - 1][3][0] = (Position) { 0, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][3][1] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][3][2] = (Position) { 2, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][3][3] = (Position) { 3, 1 };
    //     Block_Move(block, -1, 3);
    //     break;
    //
    // case O:
    //     block->id = 4;
    //     block->numRotations = 1;
    //     // Only one rotation for O block
    //     BLOCK_LAYOUTS[block->id - 1][0][0] = (Position) { 0, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][0][1] = (Position) { 0, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][0][2] = (Position) { 1, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][0][3] = (Position) { 1, 1 };
    //     Block_Move(block, 0, 4);
    //     break;
    //
    // case S:
    //     block->id = 5;
    //     block->numRotations = 4;
    //     // Rotation 0
    //     BLOCK_LAYOUTS[block->id - 1][0][0] = (Position) { 0, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][0][1] = (Position) { 0, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][0][2] = (Position) { 1, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][0][3] = (Position) { 1, 1 };
    //     // Rotation 1
    //     BLOCK_LAYOUTS[block->id - 1][1][0] = (Position) { 0, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][1][1] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][1][2] = (Position) { 1, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][1][3] = (Position) { 2, 2 };
    //     // Rotation 2
    //     BLOCK_LAYOUTS[block->id - 1][2][0] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][2][1] = (Position) { 1, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][2][2] = (Position) { 2, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][2][3] = (Position) { 2, 1 };
    //     // Rotation 3
    //     BLOCK_LAYOUTS[block->id - 1][3][0] = (Position) { 0, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][3][1] = (Position) { 1, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][3][2] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][3][3] = (Position) { 2, 1 };
    //     Block_Move(block, 0, 3);
    //     break;
    //
    // case T:
    //     block->id = 6;
    //     block->numRotations = 4;
    //     // Rotation 0
    //     BLOCK_LAYOUTS[block->id - 1][0][0] = (Position) { 0, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][0][1] = (Position) { 1, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][0][2] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][0][3] = (Position) { 1, 2 };
    //     // Rotation 1
    //     BLOCK_LAYOUTS[block->id - 1][1][0] = (Position) { 0, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][1][1] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][1][2] = (Position) { 1, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][1][3] = (Position) { 2, 1 };
    //     // Rotation 2
    //     BLOCK_LAYOUTS[block->id - 1][2][0] = (Position) { 1, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][2][1] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][2][2] = (Position) { 1, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][2][3] = (Position) { 2, 1 };
    //     // Rotation 3
    //     BLOCK_LAYOUTS[block->id - 1][3][0] = (Position) { 0, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][3][1] = (Position) { 1, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][3][2] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][3][3] = (Position) { 2, 1 };
    //     Block_Move(block, 0, 3);
    //     break;
    //
    // case Z:
    //     block->id = 7;
    //     block->numRotations = 4;
    //     // Rotation 0
    //     BLOCK_LAYOUTS[block->id - 1][0][0] = (Position) { 0, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][0][1] = (Position) { 0, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][0][2] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][0][3] = (Position) { 1, 2 };
    //     // Rotation 1
    //     BLOCK_LAYOUTS[block->id - 1][1][0] = (Position) { 0, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][1][1] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][1][2] = (Position) { 1, 2 };
    //     BLOCK_LAYOUTS[block->id - 1][1][3] = (Position) { 2, 1 };
    //     // Rotation 2
    //     BLOCK_LAYOUTS[block->id - 1][2][0] = (Position) { 1, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][2][1] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][2][2] = (Position) { 2, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][2][3] = (Position) { 2, 2 };
    //     // Rotation 3
    //     BLOCK_LAYOUTS[block->id - 1][3][0] = (Position) { 0, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][3][1] = (Position) { 1, 0 };
    //     BLOCK_LAYOUTS[block->id - 1][3][2] = (Position) { 1, 1 };
    //     BLOCK_LAYOUTS[block->id - 1][3][3] = (Position) { 2, 0 };
    //     Block_Move(block, 0, 3);
    //     break;
    //
    // default:
    //     fprintf(stderr, "Unknown block type: %c\n", type);
    //     free(block);
    //     return NULL;
    // }

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