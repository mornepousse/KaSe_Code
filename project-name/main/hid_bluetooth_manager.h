#ifndef __HID_BLUETOOTH_MANAGER__
#define __HID_BLUETOOTH_MANAGER__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include "esp_err.h"
#include "esp_mac.h"
#ifdef __cplusplus
extern "C" {
#endif
extern void init_hid_bluetooth(void);
extern void send_hid_bl_key(uint8_t keycodes[6]);

#ifdef __cplusplus
}
#endif
#endif /* HID_BLUETOOTH_MANAGER__ */