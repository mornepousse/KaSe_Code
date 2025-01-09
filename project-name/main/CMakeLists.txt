idf_component_register(
    SRCS "main.c" "keypress_handles.c" "keymap.c" "matrix.c" "screen_manager.c" "esp_hidd_prf_api.c"
                            "hid_dev.c"
                            "hid_device_le_prf.c" "hid_bluetooth_manager.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES esp_wifi esp_driver_gpio esp_timer bt esp_hid nvs_flash spiffs )
