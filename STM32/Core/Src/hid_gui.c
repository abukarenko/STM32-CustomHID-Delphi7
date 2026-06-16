#include "hid_gui.h"
#include "gui_state.h"
#include "usbd_customhid.h"
#include "usb_device.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

static uint8_t calc_checksum(uint8_t *buf)
{
    uint8_t sum = 0;

    for (uint8_t i = 0; i < 7; i++)
        sum += buf[i];

    return sum;
}

void SendGuiStateToPC(void)
{
    uint8_t report[8];

    report[0] = 0x01;              // Packet type: GUI_STATE

    report[1] = GuiState.value1;
    report[2] = GuiState.value2;
    report[3] = GuiState.value3;

    report[4] = GuiState.sw1;
    report[5] = GuiState.sw2;
    report[6] = GuiState.sw3;

    report[7] = calc_checksum(report);

    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, 8);
}
