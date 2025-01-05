/*
 * keymap.h
 *
 *  Created on: 20 Sep 2018
 *      Author: gal
 */

#ifndef MAIN_KEYMAP_H_
#define MAIN_KEYMAP_H_

#include "keyboard_config.h"
extern uint16_t (*default_layouts[])[MATRIX_ROWS][KEYMAP_COLS];
extern uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS];
extern uint16_t _PLUGINS[MATRIX_ROWS][KEYMAP_COLS];
#endif /* MAIN_KEYMAP_H_ */