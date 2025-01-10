#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "driver/gpio.h"
#include "matrix.h"
#include "keypress_handles.c"
#include "keyboard_config.h"
#include "lvgl.h"
#include "screen_manager.h"
#include "hid_bluetooth_manager.h"

char **layer_names_arr;

uint8_t layers_num = 0;
TaskHandle_t xKeyreportTask;
uint8_t interval = 10;
uint8_t clignot = 0;
uint8_t current_interval = 0;
uint8_t current_row_layer_changer = 255;
uint8_t current_col_layer_changer = 255;
uint8_t last_layer = 0;
#define APP_BUTTON (GPIO_NUM_0) // Use BOOT signal by default
#define CURSOR_LED_RED_PIN (GPIO_NUM_8)
#define CURSOR_LED_GRN_PIN (GPIO_NUM_17)
#define CURSOR_LED_BLU_PIN (GPIO_NUM_18)
#define CURSOR_LED_WHT_PIN (GPIO_NUM_16)
#define CURSOR_UP (GPIO_NUM_7)
#define CURSOR_DWN (GPIO_NUM_15)
#define CURSOR_LFT (GPIO_NUM_5)
#define CURSOR_RHT (GPIO_NUM_6)

#define TEST_LCD_HOST               SPI2_HOST
#define TEST_LCD_H_RES              (240)
#define TEST_LCD_V_RES              (240)
#define TEST_LCD_BIT_PER_PIXEL      (16)

#define TEST_PIN_NUM_LCD_CS         (GPIO_NUM_1)
#define TEST_PIN_NUM_LCD_PCLK       (GPIO_NUM_41)
#define TEST_PIN_NUM_LCD_DATA0      (GPIO_NUM_42)
#define TEST_PIN_NUM_LCD_RST        (GPIO_NUM_44)
#define TEST_PIN_NUM_LCD_DC         (GPIO_NUM_2)

#define TEST_DELAY_TIME_MS          (3000)

uint8_t cursor_states[4] = {0, 0, 0, 0};//up, down, left, right
uint8_t keypress_internal_function = 0;
/************* BL ****************/
#define CONFIG_BT_HID_DEVICE_ENABLED 1
uint8_t bl_state = 0;
uint8_t usb_bl_state = 0; // 0: USB, 1: BL
/************* BL ****************/

static const char *TAG = "Main";

/************* TinyUSB descriptors ****************/

#define TUSB_DESC_TOTAL_LEN (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)

/**
 * @brief HID report descriptor
 *
 * In this example we implement Keyboard + Mouse HID device,
 * so we must define both report descriptors
 */
const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_ITF_PROTOCOL_KEYBOARD)),
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(HID_ITF_PROTOCOL_MOUSE))};

/**
 * @brief String descriptor
 */
const char *hid_string_descriptor[5] = {
    // array of pointer to string descriptors
    (char[]){0x09, 0x04}, // 0: is supported language is English (0x0409)
    "Mae",                // 1: Manufacturer
    "Mae Keyboard",       // 2: Product
    "1994",               // 3: Serials, should use chip ID
    "Keyboard",           // 4: HID
};

/**
 * @brief Configuration descriptor
 *
 * This is a simple configuration descriptor that defines 1 configuration and 1 HID interface
 */
static const uint8_t hid_configuration_descriptor[] = {
    // Configuration number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    // Interface number, string index, boot protocol, report descriptor len, EP In address, size & polling interval
    TUD_HID_DESCRIPTOR(0, 4, false, sizeof(hid_report_descriptor), 0x81, 16, 10),
};

/********* TinyUSB HID callbacks ***************/

// Invoked when received GET HID REPORT DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
    // We use only one interface and one HID report descriptor, so we can ignore parameter 'instance'
    return hid_report_descriptor;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
}

/********* Application ***************/

typedef enum
{
    MOUSE_DIR_RIGHT,
    MOUSE_DIR_DOWN,
    MOUSE_DIR_LEFT,
    MOUSE_DIR_UP,
    MOUSE_DIR_MAX,
} mouse_dir_t;

#define DISTANCE_MAX 125
#define DELTA_SCALAR 5

void send_hid_key(uint8_t keycodes[6])
{
    if(usb_bl_state == 0)
                {
                    if (tud_mounted())
                        tud_hid_keyboard_report(1, 0, keycodes);
                }
                else
                {
                    send_hid_bl_key(keycodes);
                }
}



uint8_t cursor_pressed_state = 0; 
void getLvltrackball(uint8_t CURSOR_Dir, uint8_t key, uint8_t cursor_Selected_state)
{
    uint8_t state = gpio_get_level(CURSOR_Dir);
    if (cursor_states[cursor_Selected_state] != state)
    {

        cursor_states[cursor_Selected_state] = state;
        if (state == 1)
        {

            for (uint8_t i = 0; i < 6; i++)
            {
                if (keycodes[0] == 0)
                {
                    keycodes[0] = key;
                    break;
                }
            }

            //tud_hid_keyboard_report(1, 0, keycodes);
            send_hid_key(keycodes);
            if (cursor_pressed_state == 0)
            {
                cursor_pressed_state = 1;
            }
        }
        else
        {
            for (uint8_t i = 0; i < 6; i++)
            {
                if (keycodes[0] == key)
                {
                    keycodes[0] = 0;
                    break;
                }
            }
            cursor_pressed_state = 0;
            send_hid_key(keycodes);
            //tud_hid_keyboard_report(1, 0, keycodes);
        }
    }
}

void vTaskTrackBall(void *pvParameters) // ICSH044A
{
    for (;;)
    {
        //ESP_LOGI(TAG, "test %d", gpio_get_level(CURSOR_UP));

        if (tud_mounted())
        {
            getLvltrackball(CURSOR_UP, KC_UP, 0);
            getLvltrackball(CURSOR_DWN, KC_DOWN, 1);
            getLvltrackball(CURSOR_LFT, KC_LEFT, 2);
            getLvltrackball(CURSOR_RHT, KC_RIGHT, 3);

            if (cursor_pressed_state != 0)
            {
                cursor_pressed_state++;
                ESP_LOGI(TAG, "state cursor");
            }
            if (cursor_pressed_state >= 100)
            {
                ESP_LOGI(TAG, "cursor_pressed_state %d", cursor_pressed_state);
                cursor_pressed_state = 0;
                for (uint8_t i = 0; i < 6; i++)
                {
                    if (keycodes[i] == KC_UP || keycodes[i] == KC_DOWN || keycodes[i] == KC_LEFT || keycodes[i] == KC_RIGHT)
                    {
                        keycodes[i] = 0;
                    }
                }
            }
            send_hid_key(keycodes);
            //tud_hid_keyboard_report(1, 0, keycodes);
        }

        // Get the next x and y delta in the draw square pattern
        // mouse_draw_square_next_delta(&delta_x, &delta_y);
        // tud_hid_mouse_report(HID_ITF_PROTOCOL_MOUSE, 0x00, delta_x, delta_y, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(4));
    }
}

void vTaskKeyboard(void *pvParameters)
{
    for (;;)
    {
        // Task code goes here.
        scan_matrix();
        for (uint8_t i = 0; i < 6; i++)
        {
            if (current_press_col[i] != 255)
            {
                uint8_t keycodeTMP = keymaps[current_layout][current_press_row[i]][current_press_col[i]];

                switch (keycodeTMP)
                {
                case KC_INT1:
                last_layer = current_layout;
                    current_layout = 1;
                    current_row_layer_changer = current_press_row[i];
                    current_col_layer_changer = current_press_col[i];
                break;
                case KC_INT2:
                case KC_INT3:
                    keypress_internal_function = keycodeTMP;
                    break;
                default:
                    if (current_row_layer_changer == current_press_row[i] && current_col_layer_changer == current_press_col[i])
                    {
                    }
                    else
                    {

                        if (keycodeTMP == KC_NO)
                        {
                            keycodeTMP = keymaps[last_layer][current_press_row[i]][current_press_col[i]];
                        }

                        keycodes[i] = keycodeTMP;
                    }
                    break;
                }
            }
            else
            {

                keycodes[i] = 0;
            }
        }
        if (current_layout != last_layer)
        {
            uint8_t changer = 0;
            for (uint8_t i = 0; i < 6; i++)
            {
                if (current_press_col[i] == current_col_layer_changer && current_press_row[i] == current_row_layer_changer)
                {
                    changer = 1;
                    break;
                }
            }

            if (changer == 0)
            {
                current_layout = last_layer;
                current_col_layer_changer = 255;
                current_row_layer_changer = 255;
            }
        }

        
        

            if (stat_matrix_changed == 1)
            {
                stat_matrix_changed = 0;
                if(keypress_internal_function != 0)
                {
                    switch (keypress_internal_function)
                    {
                case KC_INT2:
                    if(bl_state == 0)
                    {
                        bl_state = 1;
                        gpio_set_level(CURSOR_LED_BLU_PIN, 1);
                        //halBLEInit(1, 1, 1, 0);
                    }
                    else
                    {
                        bl_state = 0;
                        gpio_set_level(CURSOR_LED_BLU_PIN, 0);
                    }
                    break;
                case KC_INT3:
                    if(usb_bl_state == 0)
                    {
                        usb_bl_state = 1;
                        gpio_set_level(CURSOR_LED_BLU_PIN, 1);
                    }
                    else
                    {
                        usb_bl_state = 0;
                        gpio_set_level(CURSOR_LED_BLU_PIN, 0);
                    }
                    break;
                    default:
                        break;
                    }
                    keypress_internal_function = 0;
                }
                
                // ESP_LOGI(TAG, "layer: %d : %d %d %d %d %d %d ",current_layout, keycodes[0], keycodes[1], keycodes[2], keycodes[3], keycodes[4], keycodes[5]);
                send_hid_key(keycodes);
            }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


void app_main(void)
{
    // Initialize button that will trigger HID reports
    const gpio_config_t boot_button_config = {
        .pin_bit_mask = BIT64(APP_BUTTON),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = true,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&boot_button_config));

    ESP_LOGI(TAG, "USB initialization");
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = hid_string_descriptor,
        .string_descriptor_count = sizeof(hid_string_descriptor) / sizeof(hid_string_descriptor[0]),
        .external_phy = false,
        .configuration_descriptor = hid_configuration_descriptor,
    };

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "USB initialization DONE");

    // Reset the rtc GPIOS
    rtc_matrix_deinit();
    matrix_setup();

    //Init_gc9a01();
    //init_bl_mn();
    
    gpio_set_direction(CURSOR_LED_RED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(CURSOR_LED_GRN_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(CURSOR_LED_BLU_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(CURSOR_LED_WHT_PIN, GPIO_MODE_OUTPUT);

    gpio_set_level(CURSOR_LED_RED_PIN, 1);
    gpio_set_level(CURSOR_LED_GRN_PIN, 0);
    gpio_set_level(CURSOR_LED_BLU_PIN, 0);
    gpio_set_level(CURSOR_LED_WHT_PIN, 0);

    gpio_set_direction(CURSOR_UP,  GPIO_MODE_INPUT);
    gpio_set_direction(CURSOR_DWN, GPIO_MODE_INPUT);
    gpio_set_direction(CURSOR_LFT, GPIO_MODE_INPUT);
    gpio_set_direction(CURSOR_RHT, GPIO_MODE_INPUT);

    TaskHandle_t xHandleMatrix_Keybord = NULL;
    TaskHandle_t xHandleTrackBall = NULL;
    static uint8_t ucParameterToPass;
    xTaskCreate(vTaskKeyboard, "Matrix_Keyboard", 4096, &ucParameterToPass, tskIDLE_PRIORITY, &xHandleMatrix_Keybord);
    xTaskCreate(vTaskTrackBall, "TrackBall", 4096, &ucParameterToPass, tskIDLE_PRIORITY, &xHandleTrackBall);
    init_hid_bluetooth();

    while (1)
    {
        current_interval++;
            if (current_interval > interval)
            {
                current_interval = 0;
                if (clignot == 0)
                {
                    clignot = 1;
                }
                else
                {
                    clignot = 0;
                }
                // ESP_LOGI("HIDD", "MAIN finished...");
            }
        if (tud_mounted())
        {
            static bool send_hid_data = true;
            if (send_hid_data)
            {

                // app_send_hid_demo();
                // xTaskCreatePinnedToCore(key_reports, "key report task", 8192, xKeyreportTask, configMAX_PRIORITIES, NULL, 1);
            }
            // send_hid_data = !gpio_get_level(APP_BUTTON);

            
            gpio_set_level(CURSOR_LED_WHT_PIN, 0);
            //if (clignot == 1)
            //{
              //  gpio_set_level(CURSOR_LED_GRN_PIN, 0);
            //}
            //else
            //{
                gpio_set_level(CURSOR_LED_GRN_PIN, 1);
            //}
        }
        if (tud_suspended())
        {
            gpio_set_level(CURSOR_LED_GRN_PIN, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
