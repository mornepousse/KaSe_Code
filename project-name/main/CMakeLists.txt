idf_component_register(
    SRCS "main.c" "keymap.c" "matrix.c" "esp_hidd_prf_api.c" "hid_dev.c" "hid_device_le_prf.c" "hid_bluetooth_manager.c" "gc9a01.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES esp_wifi esp_driver_gpio esp_timer bt esp_hid nvs_flash spiffs )
