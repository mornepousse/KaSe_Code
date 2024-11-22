idf_component_register(
    SRCS "keypress_handles.c" "keymap.c" "matrix.c" "tusb_hid_example_main.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES esp_driver_gpio esp_timer
    )
