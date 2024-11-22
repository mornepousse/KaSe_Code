idf_component_register(
    SRCS "main.c" "keypress_handles.c" "keymap.c" "matrix.c" 
    INCLUDE_DIRS "."
    PRIV_REQUIRES esp_driver_gpio esp_timer
    )
