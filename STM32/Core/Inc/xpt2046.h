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
void XPT2046_SetCalibration(uint16_t raw_x_min, uint16_t raw_x_max,
                            uint16_t raw_y_min, uint16_t raw_y_max);

#endif
