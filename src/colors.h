#ifndef COLORS_H
#define COLORS_H

#include <raylib.h>

#include "array.h"

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

#endif
