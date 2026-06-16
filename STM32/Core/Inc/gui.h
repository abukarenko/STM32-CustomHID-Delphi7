#ifndef GUI_H
#define GUI_H

#include "main.h"

void GUI_Init(void);
void GUI_Draw(void);
void GUI_Touch(uint16_t x, uint16_t y);
void GUI_Task(void);
void GUI_Status(const char *txt);
void GUI_RefreshFromState(void);
#endif
