#ifndef KEYPRESS_HANDLES_C
#define KEYPRESS_HANDLES_C
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "keymap.c"
#include "matrix.h"
#include "esp_timer.h"

#define KEY_PRESS_TAG "KEY_PRESS"

/*
 * Current state of the keymap,each cell will hold the location of the key in the key report,
 *if a key is not in the report it will be set to 0
 */

uint8_t KEY_STATE[MATRIX_ROWS][KEYMAP_COLS] = { 0 };
uint8_t modifier = 0;
uint16_t keycode = 0;

// Sizing the report for N-key rollover
uint8_t current_report[REPORT_LEN] = { 0 };

// Flag in order to know when to ignore layer change on layer hold
uint8_t layer_hold_flag = 0;
uint8_t prev_layout = 0;

// checking if a modifier key was pressed
uint16_t check_modifier(uint16_t key) {

	uint8_t cur_mod = 0;
	// these are the modifier keys
	if ((KC_LCTRL <= key) && (key <= KC_RGUI)) {
		cur_mod = (1 << (key - KC_LCTRL));
		return cur_mod;
	}
	return 0;

}

//used for debouncing
static uint32_t millis() {
	return esp_timer_get_time() / 1000;
}

uint32_t prev_time = 0;
// adjust current layer
void layer_adjust(uint16_t keycode) {
	uint32_t cur_time = millis();
	if (cur_time - prev_time > DEBOUNCE) {
		if (layer_hold_flag == 0) {
			switch (keycode) {
			case DEFAULT:
				current_layout = 0;
				break;

			case LOWER:
				if (current_layout == 0) {
					current_layout = MAX_LAYER;
					break;
				}
				current_layout--;
				break;

			case RAISE:
				if (current_layout == MAX_LAYER) {
					current_layout = 0;
					break;
				}
				current_layout++;
				break;
			}
			ESP_LOGI(KEY_PRESS_TAG, "Layer modified!, Current layer: %d ", current_layout);
		}
	}
	prev_time = cur_time;
}

uint8_t matrix_prev_state[MATRIX_ROWS][MATRIX_COLS] = { 0 };

// checking the state of each key in the matrix
uint8_t *check_key_state() {

	scan_matrix();
	for (uint8_t pad = 0; pad < KEYPADS; pad++) {

		uint8_t matrix_state[MATRIX_ROWS][MATRIX_COLS] = { 0 };
		memcpy(matrix_state, matrix_states[pad], sizeof(matrix_state));

		for (uint8_t col = (MATRIX_COLS * pad); col < ((pad + 1) * (MATRIX_COLS)); col++) {
			for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
				
				//if there are no changes on this matrix position, skip to next position
				if(matrix_state[row][col] == matrix_prev_state[row][col])	
					continue;

				uint16_t report_index = (2 + col + row * KEYMAP_COLS);
				keycode = _QWERTY[row][col];

				//checking if the keycode is transparent
				if (keycode == KC_TRNS) {
					if (current_layout == 0) {
						keycode = *default_layouts[MAX_LAYER][row][col];
					} else {
						keycode =
								*default_layouts[current_layout - 1][row][col];
					}
				}

				if (matrix_state[row][col - MATRIX_COLS * pad] == 1) {

					//checking for layer hold
					if ((keycode >= LAYER_HOLD_BASE_VAL)
							&& (keycode <= LAYER_HOLD_MAX_VAL)) {
						if (layer_hold_flag == 0) {
							prev_layout = current_layout;
							current_layout = (keycode - LAYER_HOLD_BASE_VAL);
							layer_hold_flag = 1;

						//ESP_LOGI(KEY_PRESS_TAG, "Layer modified!, Current layer: %d ", current_layout);
						}

						continue;

					}

					// checking for layer adjust keycodes
					if (keycode >= LAYERS_BASE_VAL) 
					{
						layer_adjust(keycode);
						continue;
					}


					if (current_report[report_index] == 0) {
						modifier |= check_modifier(keycode);
						current_report[report_index] = keycode;

					}
				}
				if (matrix_state[row][col - MATRIX_COLS * pad] == 0) {

					//checking for layer hold release
					if ((_QWERTY[row][col] >= LAYER_HOLD_BASE_VAL)
							&& (keycode <= LAYER_HOLD_MAX_VAL)
							&& (layer_hold_flag == 1)) {
						current_layout = 0;
						layer_hold_flag = 0;

						ESP_LOGI(KEY_PRESS_TAG, "Layer modified!, Current layer: %d ", current_layout);
					}


					if (current_report[report_index] != 0) {
					

						modifier &= ~check_modifier(keycode);
						current_report[KEY_STATE[row][col]] = 0;
						current_report[report_index] = 0;

					}

				}
			}
		}
		memcpy(matrix_prev_state, matrix_state, sizeof(matrix_state));
	}
	current_report[0] = modifier;
	return current_report;

}

#endif