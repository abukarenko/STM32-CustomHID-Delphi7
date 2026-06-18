#include "gui.h"
#include "ILI9488.h"
#include "gui_state.h"

#include <stdio.h>
#include <string.h>

#define SCREEN_W        480
#define SCREEN_H        320

#define HEADER_H        40
#define STATUS_Y        286
#define STATUS_H        34

#define CARD_Y          50
#define CARD_W          140
#define CARD_H          226
#define CARD_GAP        12

#define COL1_X          18
#define COL2_X          (COL1_X + CARD_W + CARD_GAP)
#define COL3_X          (COL2_X + CARD_W + CARD_GAP)

#define CTRL_X_OFFSET   17
#define CTRL_W          106
#define BTN_H           36
#define SW_H            34

#define LABEL_Y         60
#define UP_Y            82
#define VALUE_Y         128
#define DOWN_Y          178
#define SW_Y            232

#define VALUE_BOX_H     42

#define COLOR_BG        0x1082
#define COLOR_HEADER    0x018C
#define COLOR_PANEL     0x2124
#define COLOR_PANEL_2   0x3186
#define COLOR_BORDER    0x5ACB
#define COLOR_ACCENT    0x06BF
#define COLOR_ACCENT_2  0x0479
#define COLOR_VALUE_BG  0x0000
#define COLOR_VALUE     0xFFE0
#define COLOR_OK        0x07E0
#define COLOR_OFF       0xA000
#define COLOR_STATUS    0x0010

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

static uint16_t get_card_x(uint8_t index)
{
    if (index == 0) return COL1_X;
    if (index == 1) return COL2_X;
    return COL3_X;
}

static uint16_t get_ctrl_x(uint8_t index)
{
    return get_card_x(index) + CTRL_X_OFFSET;
}

static void gui_rect(uint16_t x, uint16_t y,
                     uint16_t w, uint16_t h,
                     uint16_t color)
{
    fill_rectangle(x, y, x + w, y + h, color);
}

static void gui_frame(uint16_t x, uint16_t y,
                      uint16_t w, uint16_t h,
                      uint16_t color)
{
    gui_rect(x, y, w, 2, color);
    gui_rect(x, y + h - 2, w, 2, color);
    gui_rect(x, y, 2, h, color);
    gui_rect(x + w - 2, y, 2, h, color);
}

static void gui_text(uint16_t x, uint16_t y,
                     uint16_t color,
                     uint8_t size,
                     const char *txt)
{
    draw_string(x, y, color, size, (char*)txt);
}

static void gui_text_center(uint16_t x, uint16_t y,
                            uint16_t w,
                            uint16_t color,
                            uint8_t size,
                            const char *txt)
{
    uint16_t text_w = (uint16_t)(strlen(txt) * size * 9);
    uint16_t tx = x;

    if (text_w < w)
        tx = x + ((w - text_w) / 2);

    gui_text(tx, y, color, size, txt);
}

static void gui_draw_button(uint16_t x, uint16_t y,
                            const char *txt,
                            uint16_t fill,
                            uint16_t border)
{
    gui_rect(x, y, CTRL_W, BTN_H, fill);
    gui_frame(x, y, CTRL_W, BTN_H, border);
    gui_text_center(x, y + 8, CTRL_W, COLOR_WHITE, 2, txt);
}

static void gui_draw_header(void)
{
    gui_rect(0, 0, SCREEN_W, HEADER_H, COLOR_HEADER);
    gui_rect(0, HEADER_H - 3, SCREEN_W, 3, COLOR_ACCENT);
    gui_text(14, 10, COLOR_WHITE, 2, "USB HID CONTROL");
    gui_text(342, 14, COLOR_LIGHTCYAN, 1, "STM32F401 TFT");
}

static void gui_draw_card(uint8_t index)
{
    char label[12];
    uint16_t x = get_card_x(index);

    gui_rect(x, CARD_Y, CARD_W, CARD_H, COLOR_PANEL);
    gui_frame(x, CARD_Y, CARD_W, CARD_H, COLOR_BORDER);

    sprintf(label, "PARAM %u", (unsigned int)(index + 1));
    gui_text_center(x, LABEL_Y, CARD_W, COLOR_LIGHTCYAN, 1, label);

    gui_draw_button(x + CTRL_X_OFFSET, UP_Y, "+", COLOR_ACCENT_2, COLOR_ACCENT);
    gui_draw_button(x + CTRL_X_OFFSET, DOWN_Y, "-", COLOR_DARKBLUE, COLOR_ACCENT);

    gui_rect(x + CTRL_X_OFFSET, VALUE_Y, CTRL_W, VALUE_BOX_H, COLOR_VALUE_BG);
    gui_frame(x + CTRL_X_OFFSET, VALUE_Y, CTRL_W, VALUE_BOX_H, COLOR_PANEL_2);
}

static void gui_draw_value(uint8_t index)
{
    char buf[8];
    uint16_t x = get_ctrl_x(index);
    uint8_t *v = get_value_ptr(index);

    sprintf(buf, "%3u", (unsigned int)(*v));

    gui_rect(x + 2, VALUE_Y + 2, CTRL_W - 4, VALUE_BOX_H - 4, COLOR_VALUE_BG);
    gui_text_center(x, VALUE_Y + 7, CTRL_W, COLOR_VALUE, 3, buf);
}

static void gui_draw_switch(uint8_t index)
{
    char label[8];
    uint16_t x = get_ctrl_x(index);
    uint16_t fill;
    uint16_t border;
    uint8_t *sw = get_switch_ptr(index);

    if (*sw)
    {
        fill = COLOR_DARKGREEN;
        border = COLOR_OK;
    }
    else
    {
        fill = COLOR_DARKRED;
        border = COLOR_OFF;
    }

    gui_rect(x, SW_Y, CTRL_W, SW_H, fill);
    gui_frame(x, SW_Y, CTRL_W, SW_H, border);

    sprintf(label, "SW%u", (unsigned int)(index + 1));
    gui_text(x + 9, SW_Y + 10, COLOR_WHITE, 1, label);

    if (*sw)
        gui_text(x + 65, SW_Y + 10, COLOR_WHITE, 1, "ON");
    else
        gui_text(x + 57, SW_Y + 10, COLOR_WHITE, 1, "OFF");
}

static void gui_draw_status(const char *txt)
{
    gui_rect(0, STATUS_Y, SCREEN_W, STATUS_H, COLOR_STATUS);
    gui_rect(0, STATUS_Y, SCREEN_W, 2, COLOR_ACCENT);
    gui_text(10, STATUS_Y + 10, COLOR_WHITE, 1, txt);
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
    fill_rectangle(0, 0, SCREEN_W, SCREEN_H, COLOR_BG);
    gui_draw_header();

    gui_draw_card(0);
    gui_draw_card(1);
    gui_draw_card(2);

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
    for (uint8_t i = 0; i < 3; i++)
    {
        uint16_t ctrl_x = get_ctrl_x(i);
        uint8_t *v = get_value_ptr(i);
        uint8_t *sw = get_switch_ptr(i);

        if (hit(x, y, ctrl_x, UP_Y, CTRL_W, BTN_H))
        {
            if (*v < 255)
                (*v)++;

            gui_draw_value(i);
            gui_draw_status("VALUE UP");

            GuiStateChanged = 1;
            return;
        }

        if (hit(x, y, ctrl_x, DOWN_Y, CTRL_W, BTN_H))
        {
            if (*v > 0)
                (*v)--;

            gui_draw_value(i);
            gui_draw_status("VALUE DOWN");

            GuiStateChanged = 1;
            return;
        }

        if (hit(x, y, ctrl_x, SW_Y, CTRL_W, SW_H))
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
