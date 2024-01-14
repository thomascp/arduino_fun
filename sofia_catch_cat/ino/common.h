#ifndef COMMON_H
#define COMMON_H

#include "Arduino.h"

struct lines
{
    uint16_t x1;
    uint16_t y1;
    uint16_t x2;
    uint16_t y2;
    uint16_t color;
};

struct game_map
{
    struct lines *lines;
    uint16_t num;
};

struct pos
{
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
};

struct character
{
    struct pos pos;
    const uint16_t *pixels;
};

#define BORDER_LEFT     3
#define BORDER_RIGHT    172
#define BORDER_UP       5
#define BORDER_DOWN     214

#endif