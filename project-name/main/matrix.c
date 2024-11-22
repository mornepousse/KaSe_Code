
#ifndef MATRIX_C
#define MATRIX_C

//GPIO libraries
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "keyboard_config.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "esp_timer.h"
#define GPIO_TAG "GPIO"
/* Define pins, notice that:
 * GPIO6-11 are usually used for SPI flash
 * GPIO34-39 can only be set as input mode and do not have software pullup or pulldown functions.
 * GPIOS 0,2,4,12-15,25-27,32-39 Can be used as RTC GPIOS as well (please read about power management in ReadMe)
 */
const gpio_num_t MATRIX_ROWS_PINS[] = { GPIO_NUM_6, GPIO_NUM_7, GPIO_NUM_8, GPIO_NUM_9, GPIO_NUM_21 };
const gpio_num_t MATRIX_COLS_PINS[] = { GPIO_NUM_18, GPIO_NUM_17, GPIO_NUM_14, GPIO_NUM_13, GPIO_NUM_12, 
                                        GPIO_NUM_11,GPIO_NUM_37, GPIO_NUM_36, GPIO_NUM_35,GPIO_NUM_34, 
                                        GPIO_NUM_33,GPIO_NUM_47,GPIO_NUM_48};

// matrix states
uint8_t MATRIX_STATE[MATRIX_ROWS][MATRIX_COLS] = { 0 };
uint8_t PREV_MATRIX_STATE[MATRIX_ROWS][MATRIX_COLS] = { 0 };
uint8_t SLAVE_MATRIX_STATE[MATRIX_ROWS][MATRIX_COLS] = { 0 };

uint32_t lastDebounceTime = 0;

uint8_t (*matrix_states[])[MATRIX_ROWS][MATRIX_COLS] = { &MATRIX_STATE,
		&SLAVE_MATRIX_STATE, };

//used for debouncing
static uint32_t millis() {
	return esp_timer_get_time() / 1000;
}

// deinitializing rtc matrix pins on  deep sleep wake up
void rtc_matrix_deinit(void) {

	// Deinitializing columns
	for (uint8_t col = 0; col < MATRIX_COLS; col++) {

		if (rtc_gpio_is_valid_gpio(MATRIX_COLS_PINS[col]) == 1) {
			rtc_gpio_set_level(MATRIX_COLS_PINS[col], 0);
			rtc_gpio_set_direction(MATRIX_COLS_PINS[col],
					RTC_GPIO_MODE_DISABLED);
			gpio_reset_pin(MATRIX_COLS_PINS[col]);
		}
	}

	// Deinitializing rows
	for (uint8_t row = 0; row < MATRIX_ROWS; row++) {

		if (rtc_gpio_is_valid_gpio(MATRIX_ROWS_PINS[row]) == 1) {
			rtc_gpio_set_level(MATRIX_ROWS_PINS[row], 0);
			rtc_gpio_set_direction(MATRIX_ROWS_PINS[row],
					RTC_GPIO_MODE_DISABLED);
			gpio_reset_pin(MATRIX_ROWS_PINS[row]);
		}
	}
}

// Initializing rtc matrix pins for deep sleep wake up
void rtc_matrix_setup(void) {
	uint64_t rtc_mask = 0;

	// Initializing columns
	for (uint8_t col = 0; col < MATRIX_COLS; col++) {

		if (rtc_gpio_is_valid_gpio(MATRIX_COLS_PINS[col]) == 1) {
			rtc_gpio_init((MATRIX_COLS_PINS[col]));
			rtc_gpio_set_direction(MATRIX_COLS_PINS[col],
					RTC_GPIO_MODE_INPUT_OUTPUT);
			rtc_gpio_set_level(MATRIX_COLS_PINS[col], 1);

			ESP_LOGI(GPIO_TAG,"%d is level %d", MATRIX_COLS_PINS[col], gpio_get_level(MATRIX_COLS_PINS[col]));
		}
	}

	// Initializing rows
	for (uint8_t row = 0; row < MATRIX_ROWS; row++) {

		if (rtc_gpio_is_valid_gpio(MATRIX_ROWS_PINS[row]) == 1) {
			rtc_gpio_init((MATRIX_ROWS_PINS[row]));
			rtc_gpio_set_direction(MATRIX_ROWS_PINS[row],
					RTC_GPIO_MODE_INPUT_OUTPUT);
			rtc_gpio_set_drive_capability(MATRIX_ROWS_PINS[row],
					GPIO_DRIVE_CAP_0);
			rtc_gpio_set_level(MATRIX_ROWS_PINS[row], 0);
			rtc_gpio_wakeup_enable(MATRIX_ROWS_PINS[row], GPIO_INTR_HIGH_LEVEL);
			SET_BIT(rtc_mask, MATRIX_ROWS_PINS[row]);

			ESP_LOGI(GPIO_TAG,"%d is level %d", MATRIX_ROWS_PINS[row], gpio_get_level(MATRIX_ROWS_PINS[row]));
		}
		esp_sleep_enable_ext1_wakeup(rtc_mask, ESP_EXT1_WAKEUP_ANY_HIGH);
	}
}

// Initializing matrix pins
void matrix_setup(void) {

	// Initializing columns
	for (uint8_t col = 0; col < MATRIX_COLS; col++) {

		esp_rom_gpio_pad_select_gpio(MATRIX_COLS_PINS[col]);
		gpio_set_direction(MATRIX_COLS_PINS[col], GPIO_MODE_INPUT_OUTPUT);
		gpio_set_level(MATRIX_COLS_PINS[col], 0);

		ESP_LOGI(GPIO_TAG,"%d is level %d", MATRIX_COLS_PINS[col], gpio_get_level(MATRIX_COLS_PINS[col]));
	}

	// Initializing rows
	for (uint8_t row = 0; row < MATRIX_ROWS; row++) {

		esp_rom_gpio_pad_select_gpio(MATRIX_ROWS_PINS[row]);
		gpio_set_direction(MATRIX_ROWS_PINS[row], GPIO_MODE_INPUT_OUTPUT);
		gpio_set_drive_capability(MATRIX_ROWS_PINS[row], GPIO_DRIVE_CAP_0);
		gpio_set_level(MATRIX_ROWS_PINS[row], 0);

		ESP_LOGI(GPIO_TAG,"%d is level %d", MATRIX_ROWS_PINS[row], gpio_get_level(MATRIX_ROWS_PINS[row]));
	}

}

uint8_t curState = 0;
uint32_t DEBOUNCE_MATRIX[MATRIX_ROWS][MATRIX_COLS] = { 0 };
// Scanning the matrix for input
void scan_matrix(void) {
#ifdef COL2ROW
	// Setting column pin as low, and checking if the input of a row pin changes.
	for (uint8_t col = 0; col < MATRIX_COLS; col++) {
		gpio_set_level(MATRIX_COLS_PINS[col], 1);
		for (uint8_t row = 0; row < MATRIX_ROWS; row++) {

			curState = gpio_get_level(MATRIX_ROWS_PINS[row]);
			if (PREV_MATRIX_STATE[row][col] != curState) {
				DEBOUNCE_MATRIX[row][col] = millis();
			}
			PREV_MATRIX_STATE[row][col] = curState;
			if ((millis() - DEBOUNCE_MATRIX[row][col]) > DEBOUNCE) {

				if (MATRIX_STATE[row][col] != curState) {
					MATRIX_STATE[row][col] = curState;
				}

			}
		}
		gpio_set_level(MATRIX_COLS_PINS[col], 0);
	}

#endif
#ifdef ROW2COL
	// Setting row pin as low, and checking if the input of a column pin changes.
	for(uint8_t row=0; row < MATRIX_ROWS; row++) {
		gpio_set_level(MATRIX_ROWS_PINS[row], 1);

		for(uint8_t col=0; col <MATRIX_COLS; col++) {

			curState = gpio_get_level(MATRIX_COLS_PINS[col]);
			if( PREV_MATRIX_STATE[row][col] != curState) {
				DEBOUNCE_MATRIX[row][col] = millis();
			}
			PREV_MATRIX_STATE[row][col] = curState;
			if( (millis() - DEBOUNCE_MATRIX[row][col]) > DEBOUNCE) {

				if( MATRIX_STATE[row][col] != curState) {
					MATRIX_STATE[row][col] = curState;
				}

			}
		}
		gpio_set_level(MATRIX_ROWS_PINS[row], 0);
	}
#endif

}

#endif