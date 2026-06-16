#include "gui.h"
#include "ILI9488.h"
#include "gui_state.h"

#include <stdio.h>
#include <string.h>

#define SCREEN_W 480
#define SCREEN_H 320

#define BTN_W    90
#define BTN_H    55

#define COL1_X   45
#define COL2_X   195
#define COL3_X   345

#define UP_Y     25
#define VAL_Y    95
#define DOWN_Y   145

#define SW_Y     230
#define STATUS_Y 292

static uint8_t* get_value_ptr(uint8_t index)
{
    if (index == 0) return &GuiState.value1;
    if (index == 1) return &GuiState.value2;
    return &GuiState.value3;
}

static uint8_t* get_switch_ptr(uint8_t index)
{
    if (index == 0) return &GuiState.sw1;
    if (index == 1) return &GuiState.sw2;
    return &GuiState.sw3;
}

static void gui_rect(uint16_t x, uint16_t y,
                     uint16_t w, uint16_t h,
                     uint16_t color)
{
    fill_rectangle(x, y, x + w, y + h, color);
}

static void gui_text(uint16_t x, uint16_t y,
                     uint16_t color,
                     uint8_t size,
                     const char *txt)
{
    draw_string(x, y, color, size, (char*)txt);
}

static void gui_draw_button(uint16_t x, uint16_t y,
                            const char *txt,
                            uint16_t color)
{
    gui_rect(x, y, BTN_W, BTN_H, color);
    gui_text(x + 20, y + 18, COLOR_WHITE, 2, txt);
}

static void gui_draw_value(uint8_t index)
{
    char buf[8];
    uint16_t x;
    uint8_t *v = get_value_ptr(index);

    if (index == 0) x = COL1_X;
    else if (index == 1) x = COL2_X;
    else x = COL3_X;

    sprintf(buf, "%3u", *v);

    gui_rect(x, VAL_Y, BTN_W, 40, COLOR_BLACK);
    gui_text(x + 18, VAL_Y + 10, COLOR_YELLOW, 2, buf);
}

static void gui_draw_switch(uint8_t index)
{
    char buf[8];
    uint16_t x;
    uint16_t color;
    uint8_t *sw = get_switch_ptr(index);

    if (index == 0) x = COL1_X;
    else if (index == 1) x = COL2_X;
    else x = COL3_X;

    color = (*sw) ? COLOR_GREEN : COLOR_DARKRED;

    gui_rect(x, SW_Y, BTN_W, 45, color);

    sprintf(buf, "SW%u", index + 1);
    gui_text(x + 20, SW_Y + 5, COLOR_WHITE, 1, buf);

    if (*sw)
        gui_text(x + 25, SW_Y + 23, COLOR_WHITE, 1, "ON");
    else
        gui_text(x + 22, SW_Y + 23, COLOR_WHITE, 1, "OFF");
}

static void gui_draw_status(const char *txt)
{
    gui_rect(0, STATUS_Y, SCREEN_W, 28, COLOR_NAVY);
    gui_text(8, STATUS_Y + 8, COLOR_WHITE, 1, txt);
}

void GUI_Status(const char *txt)
{
    gui_draw_status(txt);
}

void GUI_Init(void)
{
    GUI_Draw();
}

void GUI_Draw(void)
{
    fill_rectangle(0, 0, SCREEN_W, SCREEN_H, COLOR_BLACK);

    gui_draw_button(COL1_X, UP_Y,   "UP",   COLOR_BLUE);
    gui_draw_button(COL2_X, UP_Y,   "UP",   COLOR_BLUE);
    gui_draw_button(COL3_X, UP_Y,   "UP",   COLOR_BLUE);

    gui_draw_button(COL1_X, DOWN_Y, "DOWN", COLOR_DARKBLUE);
    gui_draw_button(COL2_X, DOWN_Y, "DOWN", COLOR_DARKBLUE);
    gui_draw_button(COL3_X, DOWN_Y, "DOWN", COLOR_DARKBLUE);

    gui_draw_value(0);
    gui_draw_value(1);
    gui_draw_value(2);

    gui_draw_switch(0);
    gui_draw_switch(1);
    gui_draw_switch(2);

    gui_draw_status("USB HID READY");
}

static uint8_t hit(uint16_t tx, uint16_t ty,
                   uint16_t x, uint16_t y,
                   uint16_t w, uint16_t h)
{
    return (tx >= x && tx < (x + w) &&
            ty >= y && ty < (y + h));
}

void GUI_Touch(uint16_t x, uint16_t y)
{
    uint16_t col_x[3] = {COL1_X, COL2_X, COL3_X};

    for (uint8_t i = 0; i < 3; i++)
    {
        uint8_t *v = get_value_ptr(i);
        uint8_t *sw = get_switch_ptr(i);

        if (hit(x, y, col_x[i], UP_Y, BTN_W, BTN_H))
        {
            if (*v < 255)
                (*v)++;

            gui_draw_value(i);
            gui_draw_status("VALUE UP");

            GuiStateChanged = 1;
            return;
        }

        if (hit(x, y, col_x[i], DOWN_Y, BTN_W, BTN_H))
        {
            if (*v > 0)
                (*v)--;

            gui_draw_value(i);
            gui_draw_status("VALUE DOWN");

            GuiStateChanged = 1;
            return;
        }

        if (hit(x, y, col_x[i], SW_Y, BTN_W, 45))
        {
            *sw = !(*sw);

            gui_draw_switch(i);
            gui_draw_status("SWITCH TOGGLE");

            GuiStateChanged = 1;
            return;
        }
    }
}


void GUI_RefreshFromState(void)
{
    gui_draw_value(0);
    gui_draw_value(1);
    gui_draw_value(2);

    gui_draw_switch(0);
    gui_draw_switch(1);
    gui_draw_switch(2);

    gui_draw_status("PC UPDATE");
}


void GUI_Task(void)
{
}
