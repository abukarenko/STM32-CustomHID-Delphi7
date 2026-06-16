#include "xpt2046.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

#define TOUCH_CS_PORT GPIOB
#define TOUCH_CS_PIN  GPIO_PIN_12


#define RAW_X_MIN  220
#define RAW_X_MAX  3516
#define RAW_Y_MIN  205
#define RAW_Y_MAX  3794

uint8_t XPT2046_GetScreenPoint(uint16_t *sx, uint16_t *sy)
{
    TS_Point p = XPT2046_GetPoint();

    if (p.z <= 100)
        return 0;

    int32_t x = (int32_t)(RAW_Y_MAX - p.y) * 480 / (RAW_Y_MAX - RAW_Y_MIN);
    int32_t y = (int32_t)(RAW_X_MAX - p.x) * 320 / (RAW_X_MAX - RAW_X_MIN);

    if (x < 0) x = 0;
    if (x > 479) x = 479;

    if (y < 0) y = 0;
    if (y > 319) y = 319;

    *sx = (uint16_t)x;
    *sy = (uint16_t)y;

    return 1;
}


static void XPT2046_SetSlowSPI(void)
{
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    HAL_SPI_Init(&hspi1);
}

static void XPT2046_SetFastSPI(void)
{
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    HAL_SPI_Init(&hspi1);
}


static uint16_t XPT2046_ReadADC(uint8_t cmd)
{
    uint8_t tx[3];
    uint8_t rx[3];

    tx[0] = cmd;
    tx[1] = 0x00;
    tx[2] = 0x00;

    HAL_GPIO_WritePin(TOUCH_CS_PORT, TOUCH_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(&hspi1, tx, rx, 3, 100);
    HAL_GPIO_WritePin(TOUCH_CS_PORT, TOUCH_CS_PIN, GPIO_PIN_SET);

    return (((uint16_t)rx[1] << 8) | rx[2]) >> 3;
}

void XPT2046_Init(void)
{
    HAL_GPIO_WritePin(TOUCH_CS_PORT, TOUCH_CS_PIN, GPIO_PIN_SET);
}

uint8_t XPT2046_Touched(void)
{
    TS_Point p = XPT2046_GetPoint();

    if (p.z > 100)
        return 1;

    return 0;
}

TS_Point XPT2046_GetPoint(void)
{
    TS_Point p;

    XPT2046_SetSlowSPI();

    uint16_t x1 = XPT2046_ReadADC(0xD0);
    uint16_t x2 = XPT2046_ReadADC(0xD0);
    uint16_t y1 = XPT2046_ReadADC(0x90);
    uint16_t y2 = XPT2046_ReadADC(0x90);

    XPT2046_SetFastSPI();

    p.x = (x1 + x2) / 2;
    p.y = (y1 + y2) / 2;

    if (p.x > 2800 && p.y > 3800)
        p.z = 0;
    else if (p.x > 50 && p.y > 50)
        p.z = 500;
    else
        p.z = 0;

    return p;
}
