#include "tetris.h"
#include <stdlib.h>
#include <string.h>

// allocates on the heap. must be freed
Array Block_GetCellPositions(const Block* block)
{
    Array tiles = *(Array*)Array_Get(&block->cells, block->rotationState);
    Array movedTiles = Array_Init(Array_Size(&tiles), sizeof(Position));
    for (size_t i = 0; i < Array_Size(&tiles); i++) {
        Position oldPosition = *(Position*)Array_Get(&tiles, i);
        Position newPosition = Position_Init(oldPosition.row + block->rowOffset, oldPosition.column + block->columnOffset);
        Array_Set(&movedTiles, i, &newPosition);
    }

    return movedTiles;
}

void Block_Draw(const Block* block, int offsetX, int offsetY)
{
    Array tiles = Block_GetCellPositions(block);
    for (size_t i = 0; i < Array_Size(&tiles); i++) {
        Position position = *(Position*)Array_Get(&tiles, i);
        Color color = *(Color*)Array_Get(&block->colors, block->id);
        DrawRectangle(position.column * block->cellSize + offsetX, position.row * block->cellSize + offsetY,
            block->cellSize - 1, block->cellSize - 1, color);
    }
    Array_Free(&tiles); // because tiles is malloced
}

void Block_Move(Block* block, int rows, int columns)
{
    block->rowOffset += rows;
    block->columnOffset += columns;
}

void Block_Rotate(Block* block)
{
    block->rotationState++;
    if (block->rotationState == (uint8_t)Array_Size(&block->cells))
        block->rotationState = 0;
}

void Block_UndoRotation(Block* block)
{
    if (block->rotationState == 0)
        block->rotationState = (uint8_t)Array_Size(&block->cells) - 1;
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
    block->colors = GetCellColors();

    switch (type) {
    case 'L':
        block->id = 1;
        block->cells = Array_Init( 4, sizeof(Array));
        {
            const Position cells[4][4] = {
                { (Position) { 0, 2 }, (Position) { 1, 0 }, (Position) { 1, 1 }, (Position) { 1, 2 } },
                { (Position) { 0, 1 }, (Position) { 1, 1 }, (Position) { 2, 1 }, (Position) { 2, 2 } },
                { (Position) { 1, 0 }, (Position) { 1, 1 }, (Position) { 1, 2 }, (Position) { 2, 0 } },
                { (Position) { 0, 0 }, (Position) { 0, 1 }, (Position) { 1, 1 }, (Position) { 2, 1 } },
            };
            for (size_t i = 0; i < Array_Size(&block->cells); i++) {
                Array positions = Array_Init(4, sizeof(Position));
                for (size_t j = 0; j < Array_Size(&positions); j++) {
                    Array_Set(&positions, j, &cells[i][j]);
                }
                Array_Set(&block->cells, i, &positions);
            }
        }
        Block_Move(block, 0, 3);
        break;
    case 'J':
        block->id = 2;
        block->cells = Array_Init(4, sizeof(Array));
        {
            const Position cells[4][4] = {
                { (Position) { 0, 0 }, (Position) { 1, 0 }, (Position) { 1, 1 }, (Position) { 1, 2 } },
                { (Position) { 0, 1 }, (Position) { 0, 2 }, (Position) { 1, 1 }, (Position) { 2, 1 } },
                { (Position) { 1, 0 }, (Position) { 1, 1 }, (Position) { 1, 2 }, (Position) { 2, 2 } },
                { (Position) { 0, 1 }, (Position) { 1, 1 }, (Position) { 2, 0 }, (Position) { 2, 1 } },
            };
            for (size_t i = 0; i < Array_Size(&block->cells); i++) {
                Array positions = Array_Init(4, sizeof(Position));
                for (size_t j = 0; j < Array_Size(&positions); j++) {
                    Array_Set(&positions, j, &cells[i][j]);
                }
                Array_Set(&block->cells, i, &positions);
            }
        }
        Block_Move(block, 0, 3);
        break;
    case 'I':
        block->id = 3;
        block->cells = Array_Init(4, sizeof(Array));
        {
            const Position cells[4][4] = {
                { (Position) { 1, 0 }, (Position) { 1, 1 }, (Position) { 1, 2 }, (Position) { 1, 3 } },
                { (Position) { 0, 2 }, (Position) { 1, 2 }, (Position) { 2, 2 }, (Position) { 3, 2 } },
                { (Position) { 2, 0 }, (Position) { 2, 1 }, (Position) { 2, 2 }, (Position) { 2, 3 } },
                { (Position) { 0, 1 }, (Position) { 1, 1 }, (Position) { 2, 1 }, (Position) { 3, 1 } },
            };
            for (size_t i = 0; i < Array_Size(&block->cells); i++) {
                Array positions = Array_Init(4, sizeof(Position));
                for (size_t j = 0; j < Array_Size(&positions); j++) {
                    Array_Set(&positions, j, &cells[i][j]);
                }
                Array_Set(&block->cells, i, &positions);
            }
        }
        Block_Move(block, -1, 3);
        break;
    case 'O':
        block->id = 4;
        block->cells = Array_Init(1, sizeof(Array));
        {
            const Position cells[1][4] = {
                { (Position) { 0, 0 }, (Position) { 0, 1 }, (Position) { 1, 0 }, (Position) { 1, 1 } },
            };
            for (size_t i = 0; i < Array_Size(&block->cells); i++) {
                Array positions = Array_Init(4, sizeof(Position));
                for (size_t j = 0; j < Array_Size(&positions); j++) {
                    Array_Set(&positions, j, &cells[i][j]);
                }
                Array_Set(&block->cells, i, &positions);
            }
        }
        Block_Move(block, 0, 4);
        break;
    case 'S':
        block->id = 5;
        block->cells = Array_Init(4, sizeof(Array));
        {
            const Position cells[4][4] = {
                { (Position) { 0, 1 }, (Position) { 0, 2 }, (Position) { 1, 0 }, (Position) { 1, 1 } },
                { (Position) { 0, 1 }, (Position) { 1, 1 }, (Position) { 1, 2 }, (Position) { 2, 2 } },
                { (Position) { 1, 1 }, (Position) { 1, 2 }, (Position) { 2, 0 }, (Position) { 2, 1 } },
                { (Position) { 0, 0 }, (Position) { 1, 0 }, (Position) { 1, 1 }, (Position) { 2, 1 } },
            };
            for (size_t i = 0; i < Array_Size(&block->cells); i++) {
                Array positions = Array_Init(4, sizeof(Position));
                for (size_t j = 0; j < Array_Size(&positions); j++) {
                    Array_Set(&positions, j, &cells[i][j]);
                }
                Array_Set(&block->cells, i, &positions);
            }
        }
        Block_Move(block, 0, 3);
        break;
    case 'T':
        block->id = 6;
        block->cells = Array_Init(4, sizeof(Array));
        {
            const Position cells[4][4] = {
                { (Position) { 0, 1 }, (Position) { 1, 0 }, (Position) { 1, 1 }, (Position) { 1, 2 } },
                { (Position) { 0, 1 }, (Position) { 1, 1 }, (Position) { 1, 2 }, (Position) { 2, 1 } },
                { (Position) { 1, 0 }, (Position) { 1, 1 }, (Position) { 1, 2 }, (Position) { 2, 1 } },
                { (Position) { 0, 1 }, (Position) { 1, 0 }, (Position) { 1, 1 }, (Position) { 2, 1 } },
            };
            for (size_t i = 0; i < Array_Size(&block->cells); i++) {
                Array positions = Array_Init(4, sizeof(Position));
                for (size_t j = 0; j < Array_Size(&positions); j++) {
                    Array_Set(&positions, j, &cells[i][j]);
                }
                Array_Set(&block->cells, i, &positions);
            }
        }
        Block_Move(block, 0, 3);
        break;
    case 'Z':
        block->id = 7;
        block->cells = Array_Init(4, sizeof(Array));
        {
            const Position cells[4][4] = {
                { (Position) { 0, 0 }, (Position) { 0, 1 }, (Position) { 1, 1 }, (Position) { 1, 2 } },
                { (Position) { 0, 2 }, (Position) { 1, 1 }, (Position) { 1, 2 }, (Position) { 2, 1 } },
                { (Position) { 1, 0 }, (Position) { 1, 1 }, (Position) { 2, 1 }, (Position) { 2, 2 } },
                { (Position) { 0, 1 }, (Position) { 1, 0 }, (Position) { 1, 1 }, (Position) { 2, 0 } },
            };
            for (size_t i = 0; i < Array_Size(&block->cells); i++) {
                Array positions = Array_Init(4, sizeof(Position));
                for (size_t j = 0; j < Array_Size(&positions); j++) {
                    Array_Set(&positions, j, &cells[i][j]);
                }
                Array_Set(&block->cells, i, &positions);
            }
        }
        Block_Move(block, 0, 3);
        break;
    default:
        exit(1);
    }
    return block;
}

Block* Block_Clone(const Block* src) {
    Block* block = malloc(sizeof(Block));
    block->cellSize = src->cellSize;
    block->rotationState = src->rotationState;
    block->rowOffset = src->rowOffset;
    block->columnOffset = src->columnOffset;
    block->id = src->id;
    // Deep copy colors array
    block->colors = Array_Init(src->colors.size, src->colors.elementSize);
    memcpy(block->colors.data, src->colors.data, src->colors.size * src->colors.elementSize);
    // Deep copy cells array (array of arrays)
    block->cells = Array_Init(src->cells.size, src->cells.elementSize);
    for (size_t i = 0; i < src->cells.size; i++) {
        Array* srcPositions = (Array*)Array_Get(&src->cells, i);
        Array positions = Array_Init(srcPositions->size, srcPositions->elementSize);
        memcpy(positions.data, srcPositions->data, srcPositions->size * srcPositions->elementSize);
        Array_Set(&block->cells, i, &positions);
    }
    return block;
}

void Block_Free(Block* block) {
    for (size_t i = 0; i < block->cells.size; i++) {
        Array* positions = (Array*)Array_Get(&block->cells, i);
        Array_Free(positions);
    }
    Array_Free(&block->cells);
    Array_Free(&block->colors);
    free(block);
}