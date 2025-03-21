#include "keyboard_manager.h"
#include <stdlib.h>
#include "esp_log.h"
#include "tinyusb.h"
#include "hid_bluetooth_manager.h"
#include "key_definitions.h"
#include "matrix.h"
#include "keymap.h"

static const char *KM_TAG = "Keyboard_manager";

uint8_t bl_state = 0;
uint8_t usb_bl_state = 0; // 0: USB, 1: BL
uint8_t keypress_internal_function = 0;
uint8_t current_row_layer_changer = 255;
uint8_t current_col_layer_changer = 255;

void send_hid_key(uint8_t keycodes[6])
{
    if (usb_bl_state == 0)
    {
        if (tud_mounted())
            tud_hid_keyboard_report(1, 0, keycodes);
    }
    else
    {
        send_hid_bl_key(keycodes);
    }
}

void run_internal_funct()
{
    switch (keypress_internal_function)
    {
    case K_INT2:
        if (current_layout == 2)
        {
            current_layout = 0;
            last_layer = current_layout;
            ESP_LOGI(KM_TAG, "layer: 0");
            //gpio_set_level(CURSOR_LED_WHT_PIN, 0);
        }
        else
        {
            current_layout = 2;
            last_layer = current_layout;
            ESP_LOGI(KM_TAG, "layer: 2");
            //gpio_set_level(CURSOR_LED_WHT_PIN, 1);
        }
        break;
    case K_INT3:
        if (usb_bl_state == 0)
        {
            usb_bl_state = 1;
            //gpio_set_level(CURSOR_LED_BLU_PIN, 1);
        }
        else
        {
            usb_bl_state = 0;
            //gpio_set_level(CURSOR_LED_BLU_PIN, 0);
        }
        break;
    default:
        break;
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
                uint16_t keycodeTMP = keymaps[current_layout][current_press_row[i]][current_press_col[i]];
                
                switch (keycodeTMP)
                {
                case K_INT1:
                    last_layer = current_layout;
                    current_layout = 1;
                    current_row_layer_changer = current_press_row[i];
                    current_col_layer_changer = current_press_col[i];
                    break;
                case K_INT2:
                case K_INT3:
                    if (keypress_internal_function == 0)
                    {
                        keypress_internal_function = keycodeTMP;
                    }
                default:
                    if (current_row_layer_changer == current_press_row[i] && current_col_layer_changer == current_press_col[i])
                    {
                    }
                    else
                    {
                        if (keycodeTMP == K_NO)
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

            if (keypress_internal_function != 0)
            {
                uint8_t Key_is_up = 0;
                for (uint8_t i = 0; i < 6; i++)
                {
                    if (keycodes[i] == keypress_internal_function)
                    {
                        Key_is_up = 1;
                        ESP_LOGI(KM_TAG, "pressed");
                        break;
                    }
                }
                if (Key_is_up == 0)
                {
                    run_internal_funct();
                    keypress_internal_function = 0;
                }
            }
            ESP_LOGI(KM_TAG, "layer: %d : %d %d %d %d %d %d ", current_layout, keycodes[0], keycodes[1], keycodes[2], keycodes[3], keycodes[4], keycodes[5]);
            send_hid_key(keycodes);
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
