#ifndef GUI_H
#define GUI_H

#include "main.h"
#include "gui_state.h"

void GUI_Init(void);
void GUI_Draw(void);
void GUI_Touch(uint16_t x, uint16_t y);
void GUI_Task(void);
void GUI_Status(const char *txt);
void GUI_RefreshFromState(void);
void GUI_RefreshChangedFromState(const GUI_STATE *old_state);
void GUI_StartCalibration(void);
void GUI_StopCalibration(void);
uint8_t GUI_IsCalibrationMode(void);
void GUI_CalibrationTouch(uint16_t raw_x, uint16_t raw_y);
uint8_t GUI_IsTouchTestMode(void);
void GUI_TouchTestTouch(uint16_t x, uint16_t y);
#endif
