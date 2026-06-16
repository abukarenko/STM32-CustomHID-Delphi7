#ifndef XPT2046_H
#define XPT2046_H

#include "main.h"

typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t z;
} TS_Point;

void XPT2046_Init(void);
uint8_t XPT2046_Touched(void);
TS_Point XPT2046_GetPoint(void);

uint8_t XPT2046_GetScreenPoint(uint16_t *sx, uint16_t *sy);

#endif
