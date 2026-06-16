#include "gui_state.h"

GUI_STATE GuiState =
{
    .value1 = 10,
    .value2 = 20,
    .value3 = 30,

    .sw1 = 0,
    .sw2 = 0,
    .sw3 = 0
};

uint8_t GuiStateChanged = 0;
