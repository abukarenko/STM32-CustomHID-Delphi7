#ifndef GUI_STATE_H
#define GUI_STATE_H

#include <stdint.h>

typedef struct
{
    uint8_t value1;
    uint8_t value2;
    uint8_t value3;

    uint8_t sw1;
    uint8_t sw2;
    uint8_t sw3;

} GUI_STATE;

extern GUI_STATE GuiState;
extern uint8_t GuiStateChanged;

#endif
