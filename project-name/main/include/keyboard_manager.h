#pragma once
#include <stdlib.h>

extern void send_hid_key(uint8_t keycodes[6]);
extern void vTaskKeyboard(void *pvParameters);
