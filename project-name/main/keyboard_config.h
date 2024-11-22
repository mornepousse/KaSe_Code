#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "class/hid/hid_device.h"
#include "driver/gpio.h"

const gpio_num_t MATRIX_ROWS_PINS[] = { GPIO_NUM_0, GPIO_NUM_2, GPIO_NUM_4,
		GPIO_NUM_12, GPIO_NUM_13, GPIO_NUM_14 };
const gpio_num_t MATRIX_COLS_PINS[] = { GPIO_NUM_15, GPIO_NUM_25, GPIO_NUM_26,
		GPIO_NUM_27, GPIO_NUM_32, GPIO_NUM_33 };