#ifndef KEYBOARD_CONFIG_H
#define KEYBOARD_CONFIG_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define MODULE_ID "LOLIN 32"
#define GATTS_TAG "MK32 V3.0" // The device's name
#define MAX_BT_DEVICENAME_LENGTH 40

#define MASTER  // undefine if you are not flashing the main controller


#define DEBOUNCE 4 //debounce time in ms

//Define matrix
#define KEYPADS 2 // intended in order to create a Multiple keypad split boards
#define MATRIX_ROWS 5 // For split keyboards, define rows for one side only.
#define MATRIX_COLS 13 // For split keyboards, define columns for one side only.

#define NKRO // does not work on Android and iOS!,  we can get 18KRO on those
#define LAYERS 3 // number of layers defined

//deep sleep parameters, mind that reconnecting after deep sleep might take a minute or two
#define SLEEP_MINS 45 // undefine if you do not need deep sleep, otherwise define number of minutes for deepsleep

/*
 *---------------------------- Everything below here should not be modified for standard usage----------------------
 *
 * */
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define SET_BIT(var,pos) (var |= 1LLU << pos);

#define MAX_LAYER (LAYERS-1)
#define MOD_LED_BYTES 2 //bytes for led status and modifiers
#define MACRO_LEN 3 //keys for macros
#define KEYMAP_COLS MATRIX_COLS*KEYPADS  // used for a symmetrical split keyboard
#define REPORT_LEN (MOD_LED_BYTES+MACRO_LEN+MATRIX_ROWS*KEYMAP_COLS) //size of hid reports with NKRO and room for 3 key macro
#define REPORT_COUNT_BYTES (MATRIX_ROWS*KEYMAP_COLS+MACRO_LEN)

#define PLUGIN_BASE_VAL 0x135
#define LAYER_HOLD_MAX_VAL 0x134
#define LAYER_HOLD_BASE_VAL 0x123
#define MACRO_BASE_VAL 0x103
#define LAYERS_BASE_VAL 0xFF

#define ENCODER_SIZE 4
#define MEDIA_ENCODER 0
#define MOUSE_ENCODER 1
#define KEY_ENCODER 2

typedef struct config_data {
	char bt_device_name[MAX_BT_DEVICENAME_LENGTH];
} config_data_t;

extern uint8_t current_layout;
extern uint8_t curr_led;

extern uint16_t default_encoder_map[LAYERS][ENCODER_SIZE];
extern uint16_t default_slave_encoder_map[LAYERS][ENCODER_SIZE];

#define MAX_LAYOUT_NAME_LENGTH 15
// array to hold names of layouts for oled
extern char default_layout_names[LAYERS][MAX_LAYOUT_NAME_LENGTH];

extern TaskHandle_t xKeyreportTask;

#endif
//