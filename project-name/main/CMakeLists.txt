idf_component_register(
    SRCS "tusb_hid_example_main.c" "tusb_hid_example_main.c,keyboard_config.h"
    INCLUDE_DIRS "."
    PRIV_REQUIRES esp_driver_gpio
    )
